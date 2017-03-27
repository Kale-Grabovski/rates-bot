CREATE TABLE rates (
	id serial primary key,
	pair int default 1,
	open decimal(8, 4),
	close decimal(8, 4),
	low decimal(8, 4),
	high decimal(8, 4),
	created_at timestamptz
);

ALTER TABLE rates ADD CONSTRAINT pair_created_at_unique UNIQUE (pair, created_at);
