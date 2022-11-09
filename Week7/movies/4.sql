--SELECT * FROM movies WHERE id in (SELECT movie_id FROM ratings WHERE rating = 10 AND votes > 100)
--SELECT * FROM movies WHERE id = 10544422
SELECT COUNT(*) FROM ratings WHERE rating = 10 -- AND votes > 10