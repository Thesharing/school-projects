use GSM_DB;
go

if(OBJECT_ID('MS_trg','TR') is not null)
	drop trigger MS_trg
go
create trigger MS_trg
on MS
instead of insert
as
	merge into MS as M
	using inserted as I
	on M.IMEI = I.IMEI
	when matched then 
	update set 
		M.MSISDN = I.MSISDN, M.UserName = I.UserName,
		M.MSCompany = I.MSCompany, M.GSMMspSense = I.GSMMspSense,
		M.GSMMsHeight = I.GSMMsHeight, M.GSMMspFout = I.GSMMspFout,
		M.MZone = I.MZone
	when not matched then
	insert values
		(I.IMEI, I.MSISDN, I.UserName, I.MSCompany,
		I.GSMMspSense, I.GSMMsHeight, I.GSMMspFout, I.MZone);
go


if(OBJECT_ID('MSC_trg','TR') is not null)
	drop trigger MSC_trg
go
create trigger MSC_trg
on MSC
instead of insert
as 
	merge into MSC as M
	using inserted as I
	on M.MSCID = I.MSCID
	when matched then
	update set
		M.MSCName = I.MSCName, M.MSCCompany = I.MSCCompany,
		M.MSCLongitude = I.MSCLongitude, M.MSCLatitude = I.MSCLatitude,
		M.MSCAltitude = I.MSCAltitude
	when not matched then
	insert values
		(I.MSCID, I.MSCName, I.MSCCompany, I.MSCLongitude,
		I.MSCLatitude, I.MSCAltitude);
go


if(OBJECT_ID('BSC_trg','TR') is not null)
	drop trigger BSC_trg
go
create trigger BSC_trg
on BSC
instead of insert
as
	merge into BSC as B
	using inserted as I
	on B.BSCID = I.BSCID
	when matched then 
	update set 
		B.BSCName = I.BSCName, B.BSCCompany = I.BSCCompany,
		B.Longitude = I.Longitude, B.Latitude = I.Latitude,
		B.MSCID = I.MSCID
	when not matched then
	insert values
		(I.BSCID, I.BSCName, I.BSCCompany, I.Longitude,
		I.Latitude, I.MSCID);
go


if(OBJECT_ID('BTS_trg','TR') is not null)
	drop trigger BTS_trg
go
create trigger BTS_trg
on BTS
instead of insert
as 
	merge into BTS as B
	using inserted as I
	on B.BTSName = I.BTSName
	when matched then
	update set
		B.BSCID = I.BSCID, B.Longitude = I.Longitude,
		B.Latitude = I.Latitude, B.Altitude = I.Altitude,
		B.BTSCompany = I.BTSCompany, B.BTSPower = I.BTSPower
	when not matched then
	insert values
		(I.BTSName, I.BSCID, I.Longitude, I.Latitude,
		I.Altitude, I.BTSCompany, I.BTSPower);
go


if(OBJECT_ID('CELL_trg', 'TR') is not null)
	drop trigger CELL_trg
go
create trigger CELL_trg
on CELL
instead of insert
as
	merge into CELL as C
	using inserted as I
	on C.CELLID = I.CELLID
	when matched then
	update set
		C.BTSName = I.BTSName, C.AreaName = I.AreaName,
		C.LAC = I.LAC, C.Longitude = I.Longitude,
		C.Latitude = I.Latitude, C.Direction = I.Direction,
		C.Radious = I.Radious, C.BCCH = I.BCCH
	when not matched then
	insert values
		(I.CELLID, I.BTSName, I.AreaName, I.LAC,
		I.Longitude, I.Latitude, I.Direction, I.Radious,
		I.BCCH);
go


if(OBJECT_ID('ANTENNA_trg','TR') is not null)
	drop trigger ANTENNA_trg
go
create trigger ANTENNA_trg
on ANTENNA
instead of insert
as
	merge into ANTENNA as A
	using inserted as I
	on A.CELLID = I.CELLID
	when matched then
	update set
		A.AntennaHeight = I.AntennaHeight, A.HalfPAngle = I.HalfPAngle,
		A.MaxAttenuation = I.MaxAttenuation, A.Gain = I.Gain,
		A.AntTilt = I.AntTilt, A.Pt = I.Pt,
		A.MsPwr = I.MsPwr
	when not matched then
	insert values
		(I.CELLID, I.AntennaHeight, I.HalfPAngle, I.MaxAttenuation,
		I.Gain, I.AntTilt, I.Pt, I.MsPwr);
go


if(OBJECT_ID('CELLFREQ_trg', 'TR') is not null)
	drop trigger CELLFREQ_trg
go
create trigger CELLFREQ_trg
on CELLFREQ
instead of insert
as
	merge into CELLFREQ as C
	using inserted as I
	on C.CELLID = I.CELLID and C.Freq = I.Freq
	when not matched then
	insert values
		(I.CELLID, I.Freq);
go


if(OBJECT_ID('IDLEINFO_trg', 'TR') is not null)
	drop trigger IDLEINFO_trg
go
create trigger IDLEINFO_trg
on IDLEINFO
instead of insert
as 
	merge into IDLEINFO as ID
	using inserted as I
	on ID.CELLID = I.IMEI
	when not matched then
	insert values
		(I.CELLID, I.IMEI);
go


if(OBJECT_ID('ADJACENCE_trg','TR') is not null)
	drop trigger ADJACENCE_trg
go
create trigger ADJACENCE_trg
on ADJACENCE
instead of insert
as
	select * into filt from inserted;
	delete from filt where CELLID not in (select CELLID from CELL)
		or ADJCELLID not in (select CELLID from CELL);
	merge into ADJACENCE as A
	using filt as I
	on A.CELLID = I.CELLID and A.ADJCELLID = I.ADJCELLID
	when matched then
	update set
		A.CELLLac = I.CELLLac, A.ADJCELLLac = I.ADJCELLLac
	when not matched then 
		insert values
			(I.CELLID, I.ADJCELLID, I.CELLLac, I.ADJCELLLac);
	drop table filt;
go


if(OBJECT_ID('PHONECALL_trg','TR') is not null)
	drop trigger PHONECALL_trg
go
create trigger PHONECALL_trg
on PHONECALL
instead of insert
as
	select Date_d,Time_d,CELLID,avg(NTCH) as NTCH,
		avg(Traff) as Traff,avg(Rate) as Rate,
		avg(ThTraff) as ThTraff,avg(Callnum) as Callnum,
		avg(Congsnum) as Congsnum,avg(CallCongs) as CallCongs
	into tmp
	from inserted group by Date_d,Time_d,CELLID;
	merge into PHONECALL as P
	using tmp as I
	on P.Date_d = I.Date_d and P.Time_d = I.Time_d
		and P.CELLID = I.CELLID
	when matched then
	update set
		P.NTCH = I.NTCH, P.Traff = I.Traff,
		P.Rate = I.Rate, P.ThTraff = I.ThTraff,
		P.Callnum = I.Callnum, P.Congsnum = I.Congsnum,
		P.CallCongs = I.CallCongs
	when not matched then
	insert values
		(I.Date_d, I.Time_d, I.CELLID, I.NTCH,
		I.Traff, I.Rate, I.ThTraff, I.Callnum,
		I.Congsnum, I.CallCongs);
	drop table tmp;
go


if(OBJECT_ID('RTEST_trg', 'TR') is not null)
	drop trigger RTEST_trg
go
create trigger RTEST_trg
on RTEST
instead of insert
as
	merge into RTEST as R
	using inserted as I
	on R.Keynum = I.Keynum
	when matched then
	update set
		R.CELLID = I.CELLID, R.Longitude = I.Longitude,
		R.Latitude = I.Latitude, R.RxLev = I.RxLev
	when not matched then
	insert values
		(I.Keynum, I.CELLID, I.Longitude, I.Latitude,
		I.RxLev);
go
