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

-- табличка с удаленными записями
create table del_table
(
	id int primary key not null,
	ondx int not null default 0
);

-- view с которой будет работать пользователь
create view records_view 
as 
select id, data 
from records;


create trigger on_insert
instead of insert on records_view
for each row
begin
	delete from del_table 
	where id = new.id;

	update curr_ondx 
	set value = value + 1;
	
	insert into records (id, data, ondx)
	values (new.id, new.data, (select value from curr_ondx));
end;


create trigger on_update
instead of update on records_view
for each row
begin
	delete from records_view 
	where id = old.id;

	insert into records_view (id, data)
	values (new.id, new.data);
end;


create trigger on_delete
instead of delete on records_view
for each row
begin
	delete from records 
	where id = old.id;
	
	update curr_ondx 
	set value = value + 1;
 
	insert into del_table (id, ondx)
	values (old.id, (select value from curr_ondx));
end;



