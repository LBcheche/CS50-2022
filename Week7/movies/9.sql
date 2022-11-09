--SELECT COUNT( DISTINCT (people.name))  FROM people, stars, movies WHERE people.id = stars.person_id AND stars.movie_id = movies.id AND movies.year = 2004 ORDER BY people.birth
--SELECT people.name FROM people, stars, movies WHERE people.id = stars.person_id AND stars.movie_id = movies.id AND movies.year = 2004 ORDER BY people.birth -- 21163: this consider homonyms and more than one occurrences in the same year
--SELECT DISTINCT (people.name), people.birth FROM people, stars, movies WHERE people.id = stars.person_id AND stars.movie_id = movies.id AND movies.year = 2004 ORDER BY people.birth, people.name 17965
--SELECT DISTINCT (people.name) FROM people WHERE id IN (SELECT person_id FROM stars WHERE movie_id IN (SELECT movies.id FROM movies WHERE year = 2004)) ORDER BY people.birth , people.name -- 17965: doens't consider homonyms and more than one occurrences in the same year
SELECT name FROM people WHERE id IN (SELECT person_id FROM stars WHERE movie_id IN (SELECT movies.id FROM movies WHERE year = 2004)) ORDER BY people.birth -- 18013 rows: should consider homonyms

