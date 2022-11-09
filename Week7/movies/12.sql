-- SELECT title FROM movies WHERE id IN 
-- (SELECT movie_id FROM stars WHERE person_id IN ((SELECT id FROM people WHERE name = "Johnny Depp") AND (SELECT id FROM people WHERE name = "Helena Bonham Carter")))
--SELECT title, stars.person_id, people.name FROM movies 
--JOIN stars ON stars.movie_id = movies.id
--JOIN people ON stars.person_id = people.id
--WHERE person_id IN
--SELECT id FROM people WHERE name = "Johnny Depp" UNION SELECT id FROM people WHERE name = "Helena Bonham Carter"

SELECT movies.title FROM movies
JOIN stars ON stars.movie_id = movies.id
JOIN people ON stars.person_id = people.id
WHERE people.name = "Johnny Depp"
INTERSECT
SELECT movies.title FROM movies
JOIN stars ON stars.movie_id = movies.id
JOIN people ON stars.person_id = people.id
WHERE people.name = "Helena Bonham Carter"