use GSM_DB;
go

if(OBJECT_ID('getBTSList', 'p') is not null)
	drop procedure getBTSList
go
create procedure getBTSList
as
	select BTSName from BTS;
go


if(OBJECT_ID('getBTSInfo', 'p') is not null)
	drop procedure getBTSInfo
go
create procedure getBTSInfo
(
	@BTSNAME varchar(20)
)
as
	select BSCID,Longitude,Latitude,Altitude,BTSCompany,BTSPower
	from BTS where BTSName = @BTSNAME;
go


if(OBJECT_ID('getCellIDList','p') is not null)
	drop procedure getCellIDList
go
create procedure getCellIDList
as
	select CELLID from CELL;
go


if(OBJECT_ID('getCellInfo','p') is not null)
	drop procedure getCellInfo
go
create procedure getCellInfo
(
	@CELLID integer
)
as
	select BTSName,AreaName,LAC,Longitude,
		Latitude,Direction,Radious,BCCH
	from CELL where CELLID = @CELLID;
go


if(OBJECT_ID('setHourlyPhonecallDetail','p') is not null)
	drop procedure setHourlyPhonecallDetail
go
create procedure setHourlyPhonecallDetail
(
	@ACELLID integer,
	@ADATE integer,
	@ASTIME integer,
	@AETIME integer
)
as
	if exists(select * from sys.objects 
		where name = 'HourlyDetail')
		drop table HourlyDetail;
	select Date_d as tpDate, Time_d as tpTime, NTCH as tpNTCH,
		Traff as tpTraff, ThTraff as tpThTraff, Callnum as tpCallnum,
		Congsnum as tpCongsnum
	into tmppc
	from PHONECALL
	where Date_d = @ADATE and CELLID = @ACELLID
		and Time_d between @ASTIME and @AETIME;
	update tmppc
		set tpTime = tpTime/10000;
	select tpTime as HTime, avg(tpTraff) as HAvgTraff,
		sum(tpTraff)/avg(tpNTCH) as HTraffPerLine,
		cast(sum(tpCongsnum) as real)/cast(sum(tpCallnum) as real)*100 as HAvgCongsnum,
		sum(tpThTraff)/sum(tpTraff)*100 as HThTraffRate
	into HourlyDetail
	from tmppc
	group by tpTime
	order by tpTime asc;
	drop table tmppc;
go


if(OBJECT_ID('getHourlyPhonecallDetail','p') is not null)
	drop procedure getHourlyPhonecallDetail
go
create procedure getHourlyPhonecallDetail
as
	if exists(select * from sys.objects
		where name = 'HourlyDetail')
		select * from HourlyDetail;
	else return 1;
go


if(OBJECT_ID('setMinutePhonecallDetail','p') is not null)
	drop procedure setMinutePhonecallDetail
go
create procedure setMinutePhonecallDetail
(
	@ACELLID integer,
	@ADATE integer,
	@ASTIME integer,
	@AETIME integer
)
as
	if exists(select * from sys.objects
		where name = 'MinuteDetail')
		drop table MinuteDetail;
	select Date_d as tpDate, Time_d as tpTime, NTCH as tpNTCH,
		Traff as tpTraff, ThTraff as tpThTraff, Callnum as tpCallnum,
		Congsnum as tpCongsnum
	into tmppc
	from PHONECALL
	where Date_d = @ADATE and CELLID = @ACELLID
		and Time_d between @ASTIME and @AETIME;
	update tmppc
		set tpTime = tpTime/100;
	select tpTime as MTime, avg(tpTraff) as MAvgTraff,
		sum(tpTraff)/avg(tpNTCH) as MTraffPerLine,
		cast(sum(tpCongsnum) as real)/cast(sum(tpCallnum) as real)*100 as MAvgCongsnum,
		sum(tpThTraff)/sum(tpTraff)*100 as MThTraffRate
	into MinuteDetail
	from tmppc
	group by tpTime
	order by tpTime asc;
	drop table tmppc;
go


if(OBJECT_ID('getMinutePhonecallDetail','p') is not null)
	drop procedure getMinutePhonecallDetail
go
create procedure getMinutePhonecallDetail
as
	if exists(select * from sys.objects
		where name = 'MinuteDetail')
		select * from MinuteDetail;
	else return 1;
go


if(OBJECT_ID('setQuarterPhonecallDetail','p') is not null)
	drop procedure setQuarterPhonecallDetail
go
create procedure setQuarterPhonecallDetail
(
	@ACELLID integer,
	@ADATE integer,
	@ASTIME integer,
	@AETIME integer
)
as
	if exists(select * from sys.objects
		where name = 'QuarterDetail')
		drop table QuarterDetail;
	select Date_d as tpDate, Time_d as tpTime, NTCH as tpNTCH,
		Traff as tpTraff, ThTraff as tpThTraff, Callnum as tpCallnum,
		Congsnum as tpCongsnum
	into tmppc
	from PHONECALL
	where Date_d = @ADATE and CELLID = @ACELLID
		and Time_d between @ASTIME and @AETIME;
	update tmppc
		set tpTime = (tpTime/10000)*10 + ((tpTime/100)%100)/15;
	select tpTime as QTime, avg(tpTraff) as QAvgTraff,
		sum(tpTraff)/avg(tpNTCH) as QTraffPerLine,
		cast(sum(tpCongsnum) as real)/cast(sum(tpCallnum) as real)*100 as QAvgCongsnum,
		sum(tpThTraff)/sum(tpTraff)*100 as QThTraffRate
	into QuarterDetail
	from tmppc
	group by tpTime
	order by tpTime asc;
	drop table tmppc;
go


if(OBJECT_ID('getQuarterPhonecallDetail','p') is not null)
	drop procedure getQuarterPhonecallDetail
go
create procedure getQuarterPhonecallDetail
as
	if exists(select * from sys.objects 
		where name = 'QuarterDetail')
		select * from QuarterDetail;
	else return 1;
go


if(OBJECT_ID('setCongsDetail','p') is not null)
	drop procedure setCongsDetail
go
create procedure setCongsDetail
(
	@ACONGSRATE real,
	@ADATE integer,
	@ASTIME integer,
	@AETIME integer
)
as
	if exists(select * from sys.objects
		where name = 'CongsDetail')
		drop table CongsDetail;
	select Date_d as tpDate, Time_d as tpTime, CELLID as tpCELLID,
		NTCH as tpNTCH, Traff as tpTraff, ThTraff as tpThTraff, 
		Callnum as tpCallnum, Congsnum as tpCongsnum
	into tmppc
	from PHONECALL
	where Date_d = @ADATE
		and Time_d between @ASTIME and @AETIME;
	update tmppc
		set tpTime = tpTime/10000;
	select tpTime as CTime, tpCELLID as CCELLID,
		avg(tpTraff) as CAvgTraff,
		sum(tpTraff)/avg(tpNTCH) as CTraffPerLine,
		cast(sum(tpCongsnum) as real)/cast(sum(tpCallnum) as real)*100 as CAvgCongsnum,
		sum(tpThTraff)/sum(tpTraff)*100 as CThTraffRate
	into CongsDetail
	from tmppc
	group by tpTime,tpCELLID
	having cast(sum(tpCongsnum) as real)/cast(sum(tpCallnum) as real) > @ACONGSRATE
	order by tpTime asc;
	drop table tmppc;
go


if(OBJECT_ID('getCongsDetail','p') is not null)
	drop procedure getCongsDetail
go
create procedure getCongsDetail
as
	if exists(select * from sys.objects 
		where name = 'CongsDetail')
		select * from CongsDetail;
	else return 1;
go


if(OBJECT_ID('setAdjDist','p') is not null)
	drop procedure setAdjDist
go
create procedure setAdjDist
(
	@ACELLID integer,
	@Threshold real
)
as
	if exists(select * from sys.objects
		where name = 'ADJDIST')
		drop table ADJDIST;
	declare @LOT real, @LAT real, @EARTH_RADIUS real;
	set @EARTH_RADIUS = 6378.137;
	set @LOT = (select Longitude from CELL where CELLID = @ACELLID);
	set @LAT = (select Latitude from CELL where CELLID = @ACELLID);
	select CELLID as ADJID,
		2*asin(sqrt(power(sin(@LAT*pi()/360-Latitude*pi()/360),2)+
		cos(@LAT*pi()/180)*cos(Latitude*pi()/180)*
		power(sin(@LOT*pi()/360-Longitude*pi()/360),2)))*
		@EARTH_RADIUS as Distance
	into ADJDIST
	from CELL
	where CELLID != @ACELLID;
	delete from ADJDIST where Distance >= @Threshold;
go


if(OBJECT_ID('getAdjDist','p') is not null)
	drop procedure getAdjDist
go
create procedure getAdjDist
as
	if exists(select * from sys.objects 
		where name = 'ADJDIST' and type = 'U')
		select * from ADJDIST;
	else return 1;
go
