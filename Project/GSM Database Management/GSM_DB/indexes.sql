use GSM_DB;
go

if exists(select * from sysindexes where name = 'BSC_ind')
	drop index BSC.BSC_ind
go
create nonclustered index BSC_ind
on BSC(MSCID)
with fillfactor=40;
go


if exists(select * from sysindexes where name = 'BTS_ind')
	drop index BTS.BTS_ind
go
create nonclustered index BTS_ind
on BTS(BSCID)
with fillfactor=40;
go


if exists(select * from sysindexes where name = 'CELL_F_ind')
	drop index CELL.CELL_F_ind
go
create nonclustered index CELL_F_ind
on CELL(BTSName)
with fillfactor=40;
go


if exists(select * from sysindexes where name = 'CELL_L_ind')
	drop index CELL.CELL_F_ind
go
create nonclustered index CELL_L_ind
on CELL(Longitude,Latitude)
with fillfactor=40;
go


if exists(select * from sysindexes where name = 'ANTENNA_ind')
	drop index ANTENNA.ANTENNA_ind
go
create nonclustered index ANTENNA_ind
on ANTENNA(CELLID)
with fillfactor=40;
go


if exists(select * from sysindexes where name = 'RTEST_ind')
	drop index RTEST.RTEST_ind
go
create nonclustered index RTEST_ind
on RTEST(CELLID)
with fillfactor=40;
go
