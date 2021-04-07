-- таблица с текущим счетчиком операций
create table curr_ondx (
	value int not null default 0
);

-- целевая табличка
create table records (
	id   int  primary key not null,
	data text not null,
	ondx int  not null default 0
);

-- табличка с операциями
create table records_ops (
	id int primary key,
	ondx int not null,
	operation text not null
);


create trigger records_insert
after insert on records
for each row
begin
	update curr_ondx 
	set value = value + 1;

	delete from records_ops 
	where id = new.id;
	
	insert into records_ops (id, ondx, operation)
	values(new.id, (select value from curr_ondx), 'insert');
end;


create trigger records_upd
after update on records
for each row
begin
	update curr_ondx 
	set value = value + 1;
 
	delete from records_ops 
	where id = new.id;
	
	insert into records_ops (id, ondx, operation)
	values(new.id, (select value from curr_ondx), 'update');
end;


create trigger records_del
after delete on records
for each row
begin
	update curr_ondx 
	set value = value + 1;
 
	delete from records_ops 
	where id = old.id;
	
	insert into records_ops (id, ondx, operation)
	values(old.id, (select value from curr_ondx), 'delete');
end;
