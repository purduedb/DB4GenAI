SELECT
    llm(
        format_llm_reorder(
            'Recommend movies for the user based on movie info {} and review content {}',
            m.movie_info,
            r.review_content
        )
    )
FROM
    reviews r
    JOIN movies m ON (r.rotten_tomatoes_link = m.rotten_tomatoes_link);