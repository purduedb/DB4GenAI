SELECT
    m.movie_title
FROM
    movies m
    JOIN reviews r ON r.rotten_tomatoes_link = m.rotten_tomatoes_link
WHERE
    contains(
        llm(
            format_llm_reorder(
                'Analyze whether this movie would be suitable for kids based on movie info {} and review content {}',
                m.movie_info,
                r.review_content
            )
        ),
        'Yes'
    )
    AND r.review_type = "Fresh";