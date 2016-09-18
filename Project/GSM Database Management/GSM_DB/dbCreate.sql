use master;
go
create database GSM_DB
on
primary
	(name = Arch1,
	filename = 'D:\GSMDATA\dat1.mdf',	--文件名待议
	size = 10MB,
	maxsize = 200MB,
	filegrowth = 20),
	filegroup altergroup
	(name = Arch2,
	filename = 'D:\GSMDATA\dat2.ndf',
	size = 10MB,
	maxsize = 200MB,
	filegrowth = 20),
	(name = Arch3,
	filename = 'D:\GSMDATA\dat3.ndf',
	size = 10MB,
	maxsize = 200MB,
	filegrowth = 20)
log on
	(name = GSM_log,
	filename = 'D:\GSMDATA\log.ldf',
	size = 3MB,
	maxsize = 30MB,
	filegrowth = 3MB)
go

use GSM_DB;
go

create table MS(		/*移动台MS的相关属性*/
	IMEI			varchar(15)	not null,
	MSISDN			varchar(13),
	UserName		varchar(15),
	MSCompany		varchar(20),
	GSMMspSense		real,
	GSMMsHeight		real,
	GSMMspFout		real,
	MZone			varchar(3),
	primary key(IMEI)
);
go

create table MSC(		/*MSC的配置信息*/
	MSCID			integer	not null,
	MSCName			varchar(20),
	MSCCompany		varchar(20),
	MSCLongitude	real not null,
	MSCLatitude		real not null,
	MSCAltitude 	smallint,
	primary key(MSCID)
);
go

create table BSC(		/*BSC的配置信息*/
	BSCID			integer not null,
	BSCName			varchar(20),
	BSCCompany		varchar(20),
	Longitude		real not null,
	Latitude		real not null,
	MSCID  			integer not null,
	primary key(BSCID),
	foreign key(MSCID) references MSC(MSCID)
		on delete no action
		on update cascade
);
go

create table BTS(		/*BTS的配置信息*/
	BTSName			varchar(20) not null,
	BSCID			integer not null,
	Longitude		real not null,
	Latitude		real not null,
	Altitude		smallint,
	BTSCompany		varchar(20),
	BTSPower		smallint,
	primary key(BTSName),
	foreign key(BSCID) references BSC(BSCID)
		on delete no action
		on update cascade
);
go

create table CELL(		/*小区相关信息*/
	CELLID			integer not null,
	BTSName			varchar(20) not null,
	AreaName		varchar(10),
	LAC				smallint,
	Longitude		real not null,
	Latitude		real not null,
	Direction		real,
	Radious			real,
	BCCH			smallint,
	primary key(CELLID),
	foreign key(BTSName) references BTS(BTSName)
		on delete no action
		on update no action
);
go

create table ANTENNA(	/*天线相关参数*/
	CELLID 			integer not null,
	AntennaHeight	real,
	HalfPAngle		real,
	MaxAttenuation	real,
	Gain			real,
	AntTilt			real,
	Pt				real,
	MsPwr			real,
	primary key(CELLID),
	foreign key(CELLID) references CELL(CELLID)
		on delete no action
		on update no action
);
go

create table CELLFREQ(	/*小区频点信息*/
	CELLID			integer not null,
	Freq			smallint not null,
	primary key(CELLID, Freq),
	foreign key(CELLID) references CELL(CELLID)
		on update cascade 
		on delete cascade
);
go

create table IDLEINFO(	/*小区与MS的空闲状态信息*/
	CELLID			integer not null,
	IMEI			varchar(15) not null,
	primary key(CELLID, IMEI),
	foreign key(CELLID) references CELL(CELLID)
		on delete no action
		on update no action,
	foreign key(IMEI) references MS(IMEI)
		on delete no action
		on update no action
);
go

create table ADJACENCE(	/*小区间的邻接信息*/
	CELLID			integer not null,
	ADJCELLID		integer not null,
	CELLLac			smallint,
	ADJCELLLac		smallint,
	primary key(CELLID, ADJCELLID),
	foreign key(CELLID) references CELL(CELLID)
		on delete no action
		on update no action,
	foreign key(ADJCELLID) references CELL(CELLID)
		on delete no action
		on update no action
);
go

create table PHONECALL(	/*话务信息*/
	Date_d			integer not null,
	Time_d			integer not null,
	CELLID			integer not null,
	NTCH			smallint,
	Traff			real,
	Rate			real,
	ThTraff			real,
	Callnum			integer,
	Congsnum		integer,
	CallCongs		real,
	primary key(Date_d, Time_d, CELLID),
	foreign key(CELLID) references CELL(CELLID)
		on update no action
		on delete no action
);
go

create table RTEST(		/*路测数据信息*/
	Keynum			integer not null,
	CELLID			integer not null,
	Longitude		real not null,
	Latitude		real not null,
	RxLev			real,
	primary key(Keynum),
	foreign key(CELLID) references CELL(CELLID)
		on update no action
		on delete no action
);
go
