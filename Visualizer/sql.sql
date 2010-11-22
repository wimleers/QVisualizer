DROP TABLE IF EXISTS Events;

CREATE TABLE Events (
    time_ INT,
    input_type CHAR(20),
    event_type CHAR(20),
    widget CHAR(20),
    details CHAR(20)
);