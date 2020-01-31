SELECT people.name
FROM people
WHERE people.id IN
-- DISTINCT removes duplicates (need to remove duplicates at stars id as different people may have same name)
(SELECT DISTINCT stars.person_id
FROM stars
JOIN movies ON stars.movie_id = movies.id
WHERE movies.year = 2004)
ORDER BY people.birth;
