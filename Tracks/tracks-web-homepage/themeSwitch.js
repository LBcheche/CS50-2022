
var switchElement = document.getElementById('themeSw')
var themeStyle = document.getElementById('themeStyle')
var theme = "night"

switchElement.onclick = function(){switchTheme()}

function switchTheme()
{
    if(theme == "night")
    {
        themeStyle.setAttribute('href','dayTheme.css')
        switchElement.setAttribute('src','images/nav/day-switch.png')
        theme = "day"
    }
    else
    {
        themeStyle.setAttribute('href','nightTheme.css')
        switchElement.setAttribute('src','images/nav/night-switch.png')
        theme = "night"
    }
}