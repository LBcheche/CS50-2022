import os
import re

from cs50 import SQL
from flask import Flask, flash, jsonify, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions, HTTPException, InternalServerError
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Ensure responses aren't cached
@app.after_request
def after_request(response):
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


@app.route("/")
@login_required
def index():

    cash = getUserCash()
    portfolio = getPortfolio()
    total = getTotalMoney(cash, portfolio)

    return render_template('index.html', cash = usd(cash), portfolio = portfolio, total = usd(total))

def getUserCash():

    return float(db.execute("SELECT cash FROM users WHERE id = :uid", uid = session["user_id"])[0]["cash"])

def getPortfolio():

    portfolio = db.execute("SELECT shares.ticker, shares.company_name, wallet.amount FROM shares, wallet WHERE wallet.user_id = :uid AND wallet.share_id = shares.id", uid = session["user_id"])

    for share in portfolio:
        share['current_price'] = float(lookup(share['ticker'])["price"])
        share['usd_current_price'] = usd(share['current_price'])
        share['current_total'] = float(share['amount']) * float(share['current_price'])
        share['usd_current_total'] = usd(share['current_total'])

    return portfolio

def getTotalMoney(csh, ptf):

    total = csh
    for share in ptf:
        total += float(share['current_total'])

    print(total)

    return total

@app.route("/history")
@login_required
def history():

    return render_template('history.html', history = getHistory())


def getHistory():

    history = db.execute("SELECT * FROM history WHERE user_id = :u_id ORDER BY timestamp DESC", u_id = session["user_id"])

    for order in history:
        order['usd_price'] = usd(order['price'])
        order['usd_total'] = usd(order['total'])
        order['ticker'] = db.execute("SELECT ticker FROM shares WHERE id = :s_id", s_id = order['share_id'])[0]['ticker']

    return history

@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():

    if request.method == "POST":

        if getTicker("ticker")[0] == 0:
            return getTicker("ticker")[1]

        ticker = getTicker("ticker")[1]

        if getOrderAmount("buy", "amount")[0] == 0:
            return getOrderAmount("buy", "amount")[1]

        order_amount = getOrderAmount("buy", "amount")[1]

        current_price = lookup(ticker)["price"]

        order_total = order_amount * current_price

        updateShares(ticker)

        current_amount = getCurrentAmount(ticker, "buy")[1]


        updateWallet("buy", ticker, order_amount, current_amount)

        updateCash("buy", order_total)

        updateHistory(ticker, "buy", order_amount, current_price, order_total)

        flash('Purchase Completed')
        return redirect("/")

    else:

        return render_template("buy.html")

@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    if request.method == "POST":

        if getTicker("ticker")[0] == 0:
            return getTicker("ticker")[1]

        ticker = getTicker("ticker")[1]

        if getCurrentAmount(ticker, "sell")[0] == 0:
            return getCurrentAmount(ticker, "sell")[1]

        current_amount = getCurrentAmount(ticker, "sell")[1]

        if getOrderAmount("sell","amount", current_amount)[0] == 0:
            return getOrderAmount("sell","amount", current_amount)[1]

        order_amount = getOrderAmount("sell","amount", current_amount)[1]

        updateWallet("sell", ticker, order_amount, current_amount)

        current_price = lookup(ticker)["price"]

        order_total = order_amount * current_price

        updateCash("sell", order_total)

        updateHistory(ticker, "sell", order_amount, current_price, order_total)

        flash('Sale Completed')
        return redirect("/")

    else:

        return render_template("sell.html")

def getCash(ord_tp, ord_tot):

    cash = float(db.execute("SELECT cash FROM users WHERE id = :id", id = session["user_id"])[0]["cash"])

    if ord_tp == "sell" and ord_tot > cash:
            return apology("no cash for this order", 403)

    return cash

def getTicker(tck):

    if not request.form.get(tck):
        return [0,apology("provide a stock ticker", 403)]

    ticker = request.form.get(tck)

    if not re.match(r"[A-Z][A-Z][A-Z][A-Z]", ticker):
        return [0,apology("ticker format is AAAA", 403)]

    if not lookup(ticker):
        return [0,apology("stock ticker doesn't exist", 403)]

    return [1,ticker]

def getCurrentAmount(ticker, ord_tp):

    share_id = int(db.execute("SELECT id FROM shares WHERE ticker = :tkr", tkr = ticker)[0]['id'])

    if not db.execute("SELECT amount FROM wallet WHERE user_id = :uid AND share_id = :sid", uid = session["user_id"], sid = share_id):
        if ord_tp == "sell":
            return [0,apology("you have no share for this stock", 403)]
        else:
            return [1, 0]

    return [1, db.execute("SELECT amount FROM wallet WHERE user_id = :uid AND share_id = :sid", uid = session["user_id"], sid = share_id)[0]['amount']]


def getOrderAmount(ord_tp, st_ord_amt, cur_amt = None):

    if not request.form.get(st_ord_amt):
        return [0,apology("provide a share amount", 403)]

    order_amount = int(request.form.get(st_ord_amt))

    if order_amount <= 0:
        return [0, apology("provide a positive share amount", 403)]

    if (ord_tp == "sell" and order_amount > cur_amt):
        return [0, apology("not enought shares for this order", 403)]

    return [1,order_amount]

def updateShares(tck):

    # verify if this share is already at database.
    if not db.execute("SELECT id FROM shares WHERE ticker = :ticker", ticker = tck):

        name = lookup(tck)["name"]
        db.execute("INSERT INTO shares (company_name, ticker) VALUES (?,?)",  name, tck)

def updateWallet(ord_tp, tck, ord_amt, cur_amt):

    new_amount = calculateNewAmount(ord_tp, ord_amt, cur_amt)

    print("Order Amount:", ord_amt)
    print("Current Amount:", cur_amt)
    print("New Amount:", new_amount)

    share_id = int(db.execute("SELECT id FROM shares WHERE ticker = :ticker", ticker = tck)[0]["id"])

    if ord_tp == "buy" and not db.execute("SELECT * FROM wallet WHERE user_id = :uid AND share_id = :sid", uid = session["user_id"], sid = share_id):
        db.execute("INSERT INTO wallet (user_id, share_id, amount) VALUES (?,?,?)",  session["user_id"], share_id, new_amount)

    elif ord_tp == "sell" and new_amount == 0:
        db.execute("DELETE FROM wallet WHERE user_id = :uid AND share_id = :sid", uid = session["user_id"], sid = share_id)

    else:
        db.execute("UPDATE wallet SET amount = :amt WHERE user_id = :uid AND share_id = :sid", amt = new_amount, uid = session["user_id"], sid = share_id)

def calculateNewAmount(ord_tp, ord_amt, cur_amt):

    if (ord_tp == "sell"):
        return int(cur_amt) - int(ord_amt)
    else:
        return  int(cur_amt) + int(ord_amt)


def updateCash(ord_tp, ord_tot):

    csh = getCash(ord_tp, ord_tot)

    csh = calculateNewCash(ord_tp, csh, ord_tot)

    db.execute("UPDATE users SET cash = :cash WHERE id = :uid", cash=csh, uid=session["user_id"])

def calculateNewCash(ord_tp, csh, ott):

    if(ord_tp == "sell"):
        return csh + ott
    else:
        return csh - ott

def updateHistory(tck, otype, amt, rtprice, ott):


    share_id = int(db.execute("SELECT id FROM shares WHERE ticker = :ticker", ticker = tck)[0]["id"])

    db.execute("INSERT INTO history (user_id, share_id, type, amount, price, total) VALUES (?,?,?,?,?,?)", session["user_id"], share_id, otype, amt, rtprice, ott)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = :username",
                          username=request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():

    if request.method == "POST":

        if not request.form.get("quote"):
            return apology("provide a quote symbol", 403)

        symbol = request.form.get("quote")

        if lookup(symbol):
            return render_template("quoted.html", quoted = lookup(symbol))
        else:
            return apology("quote symbol doesn't exist", 403)
    else:

        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():

    """Register user"""

    if request.method == "POST":

        if not request.form.get("username"):
            return apology("must provide user name", 403)

        elif not request.form.get("password"):
            return apology("must provide password", 403)

        elif not request.form.get("repeated_password"):
            return apology("must repeat password", 403)

        elif request.form.get("password") != request.form.get("repeated_password"):
            return apology("must repeat same password", 403)

        elif db.execute("SELECT * FROM users WHERE username = :username", username=request.form.get("username")):
            return apology("username already exists", 403)

        else:

            hashedPassword = generate_password_hash(request.form.get("password"))

            db.execute("INSERT INTO users (username, hash) VALUES (?,?)", request.form.get("username"),hashedPassword)

        flash('Registered')
        return render_template("login.html")


    else:

        return render_template("register.html")

def errorhandler(e):
    """Handle error"""

    if not isinstance(e, HTTPException):
        e = InternalServerError()
    return apology(e.name, e.code)


# Listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)
