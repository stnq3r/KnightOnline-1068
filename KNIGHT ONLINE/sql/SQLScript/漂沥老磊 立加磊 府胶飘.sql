--select DISTINCT strAccountid FROM     CONNECTTIME_LOG
--WHERE   (timeLogout > CONVERT(DATETIME, '2002-10-24 00:00:00', 102))



SELECT * INTO #TERM FROM TB_USER WHERE  strAccountId  IN 
	(SELECT DISTINCT strAccountid from [Account_Log].[dbo].[CONNECTTIME_LOG]
		WHERE  ( timeLogout > CONVERT(DATETIME, '2002-11-04 00:00:00', 102) and timeLogout < CONVERT(DATETIME, '2002-11-11 00:00:00', 102) ) )

SELECT count(*) as '��������' FROM #TERM

declare @nYear smallint, @nAge smallint, @strSocNo char(14), @strAddress char(100), @strProvince char(5)
declare @n10 smallint, @n13 smallint, @n16 smallint, @n19 smallint, @n24 smallint, @n29 smallint, @n30 smallint, @n40 smallint, @n50 smallint, @n60 smallint
declare @nMale int, @nFemale int, @nGender int
declare @nSeoul int, @nPusan int, @nUlsan int, @nKyounggi int, @nKyoungnam int, @nKyoungbuk int, @nJunbuk int, @nChungbuk int, @nJunnam int, @nChungnam int
declare @nDaejun int, @nInchun int, @nKwangju int, @nKangwon int, @nCheju int, @nDaegu int

set @n10 = 0 set @n13 = 0 set @n16 = 0 set @n19 = 0 set @n24 = 0 set @n29 = 0 set @n30 = 0 set @n40 = 0 set @n50 = 0 set @n60 = 0
set @nMale = 0 set @nFemale = 0

set @nSeoul = 0 set @nPusan = 0 set @nUlsan = 0 set @nKyounggi = 0 set @nKyoungnam = 0 set @nKyoungbuk = 0 set @nJunbuk = 0 set @nChungnam = 0 set @nChungbuk = 0 set @nJunnam = 0
set @nDaejun = 0 set @nInchun = 0 set @nKwangju = 0 set @nKangwon = 0 set @nCheju = 0 set @nDaegu = 0

DECLARE Age_Cursor CURSOR
	FOR
	SELECT strSocNo, strAddress FROM #TERM

	OPEN Age_Cursor

	FETCH NEXT FROM Age_Cursor INTO @strSocNo, @strAddress

WHILE @@FETCH_STATUS = 0
BEGIN
	SET @nYear = CONVERT( smallint, SUBSTRING( @strSocNo, 1, 2 ) )
	SET @nAge = 102 - @nYear + 1

	IF @nAge < 11  
		SET @n10 = @n10 + 1
	ELSE IF @nAge > 10 and @nAge < 14 
		SET @n13 = @n13 + 1
	ELSE IF @nAge > 13 and @nAge < 17 
		SET @n16 = @n16 + 1
	ELSE IF @nAge > 16 and @nAge < 20 
		SET @n19 = @n19 + 1
	ELSE IF @nAge > 19 and @nAge < 25
		SET @n24 = @n24 + 1
	ELSE IF @nAge > 24 and @nAge < 30 
		SET @n29 = @n29 + 1
	ELSE IF @nAge > 29 and @nAge < 40 
		SET @n30 = @n30 + 1
	ELSE IF @nAge > 39 and @nAge < 50 
		SET @n40 = @n40 + 1
	ELSE IF @nAge > 49 and @nAge < 60 
		SET @n50 = @n50 + 1
	ELSE IF @nAge > 59 
		SET @n60 = @n60 + 1

	SET @nGender = CONVERT( smallint, SUBSTRING( @strSocNo, 8, 1 ) )
	IF @nGender = 1
		SET @nMale = @nMale + 1
	ELSE IF @nGender = 2
		SET @nFemale = @nFemale + 1

	SET @strProvince = SUBSTRING( @strAddress, 1, 5 )
	IF @strProvince = '����'
		SET @nSeoul = @nSeoul + 1
	ELSE IF @strProvince = '���'
		SET @nKyounggi = @nKyounggi + 1
	ELSE IF @strProvince = '�λ�'
		SET @nPusan = @nPusan + 1
	ELSE IF @strProvince = '���'
		SET @nKyoungbuk = @nKyoungbuk + 1
	ELSE IF @strProvince = '�泲'
		SET @nKyoungnam = @nKyoungnam + 1
	ELSE IF @strProvince = '����'
		SET @nKangwon = @nKangwon + 1
	ELSE IF @strProvince = '���'
		SET @nChungbuk = @nChungbuk + 1
 	ELSE IF @strProvince = '�泲'
		SET @nChungnam = @nChungnam + 1
	ELSE IF @strProvince = '����'
		SET @nJunbuk = @nJunbuk + 1
	ELSE IF @strProvince = '����'
		SET @nJunnam = @nJunnam + 1
	ELSE IF @strProvince = '����'
		SET @nKwangju = @nKwangju + 1
	ELSE IF @strProvince = '����'
		SET @nDaejun = @nDaejun + 1
	ELSE IF @strProvince = '��õ'
		SET @nInchun = @nInchun + 1
	ELSE IF @strProvince = '����'
		SET @nCheju = @nCheju + 1
	ELSE IF @strProvince = '���'
		SET @nUlsan = @nUlsan + 1
	ELSE IF @strProvince = '�뱸'
		SET @nDaegu = @nDaegu + 1

	FETCH NEXT FROM Age_Cursor INTO @strSocNo, @strAddress
END

CLOSE Age_Cursor
DEALLOCATE Age_Cursor

SELECT @n10 as '~10', @n13 as '11~13', @n16 as '14~16', @n19 as '17~19', @n24 as '20~24', @n29 as '25~29', @n30 as '30~39', @n40 as '40~49', @n50 as '50~59', @n60 as '60~', @nMale as 'Male', @nFemale as 'Female'
SELECT @nSeoul as '����', @nKyounggi as '���', @nKangwon as '����', @nChungbuk as '���', @nChungnam as '�泲', @nKyoungbuk as '���', @nKyoungnam as '�泲', @nJunbuk as '����', @nJunnam as '����', @nCheju as '����', @nPusan as '�λ�', @nDaegu as '�뱸', @nDaejun as '����', @nKwangju as '����', @nInchun as '��õ', @nUlsan as '���'