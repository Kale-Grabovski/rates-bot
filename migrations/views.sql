BEGIN;

DROP MATERIALIZED VIEW IF EXISTS rates_day;
--DROP MATERIALIZED VIEW IF EXISTS rates_4hour;
--DROP MATERIALIZED VIEW IF EXISTS rates_hour;

-- Hours
/*CREATE MATERIALIZED VIEW rates_hour AS
WITH
  by_hour AS (
    SELECT
      MIN(low)  AS low,
      MAX(high) AS high,
      date_trunc('hour', created_at) AS created_at
    FROM rates
    GROUP BY date_trunc('hour', created_at)
    ORDER BY created_at
  ),

  open_close AS (
    SELECT 
      a.open,
      a.close,
      a.created_at
    FROM (
      SELECT 
        row_number() OVER (PARTITION BY date_trunc('hour', created_at)) AS row_number,
        first_value(open) OVER (PARTITION BY date_trunc('hour', created_at)) AS open,
        last_value(close) OVER (PARTITION BY date_trunc('hour', created_at)) AS close,
        date_trunc('hour', created_at) AS created_at
      FROM rates
      ORDER BY created_at
    ) a
    WHERE a.row_number = 1
  )

SELECT
  o.open,
	r.high,
  r.low,
  o.close,
	r.created_at
FROM by_hour r
JOIN open_close o ON r.created_at = o.created_at;*/


-- Four hours
/*CREATE MATERIALIZED VIEW rates_4hour AS
WITH
  hour_opens AS (
    SELECT open, created_at
    FROM rates_hour
    WHERE extract(hour FROM created_at) IN (SELECT unnest(array[0, 4, 8, 12, 16, 20]))
  ),
  hour_closes AS (
    SELECT close, created_at - '3 hour'::interval AS created_at
    FROM rates_hour
    WHERE extract(hour FROM created_at) IN (SELECT unnest(array[3, 7, 11, 15, 19, 23]))
  ),
  by_4hours AS (
    SELECT
      min(low) AS low,
      max(high) AS high,
      (created_at::date || ' ' || 
        trunc(extract(hour FROM created_at) / 4) * 4 
        || ':00:00')::timestamp AS created_at
    FROM rates_hour
    GROUP BY trunc(extract(hour FROM created_at) / 4), created_at::date
  )

SELECT
  o.open       AS open,
  c.close      AS close,
  h.low        AS low,
  h.high       AS high,
  h.created_at AS created_at
FROM by_4hours h
JOIN hour_opens o ON h.created_at = o.created_at
JOIN hour_closes c ON h.created_at = c.created_at
ORDER BY h.created_at;
*/

-- Days
CREATE MATERIALIZED VIEW rates_day AS
WITH
  by_days AS (
    SELECT
      min(low)         AS low,
      max(high)        AS high,
      created_at::date AS created_at
    FROM rates_hour
    GROUP BY created_at::date
  ),

  open_close AS (
    SELECT 
      a.open, 
      a.close,
      a.created_at
    FROM (
      SELECT 
        row_number()      OVER (PARTITION BY created_at::date ORDER BY created_at DESC) AS row_number,
        first_value(open) OVER (PARTITION BY created_at::date ORDER BY created_at) AS open,
        last_value(close) OVER (PARTITION BY created_at::date ORDER BY created_at DESC) AS close,
        created_at::date
      FROM rates_hour
      ORDER BY created_at
    ) a
    WHERE a.row_number = 1
  )

SELECT
  o.open       AS open,
  o.close      AS close,
  d.low        AS low,
  d.high       AS high,
  d.created_at AS created_at
FROM by_days d 
JOIN open_close o ON d.created_at = o.created_at
ORDER BY d.created_at;


COMMIT;

