SELECT
    AVG(
        TRY_CAST(
            regexp_extract(
                llm(
                    format(
                        'Rate an integer satisfaction score between 0 (bad) and 5 (good) based on {} and {}: ',
                        r.review_content,
                        m.movie_info,
                        true
                    )
                ),
                '[^0-5]*([0-5])?',
                1
            ) AS INTEGER
        )
    ) as AverageScore
FROM
    reviews r
    JOIN movies m ON r.rotten_tomatoes_link = m.rotten_tomatoes_link
GROUP BY
    m.movie_title;