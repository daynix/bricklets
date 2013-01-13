::
:: Copyright (c) 2013, Daynix Computing LTD (www.daynix.com)
:: All rights reserved.
::
:: Maintained by bricklets@daynix.com
::
:: This file is a part of Bricklets library, see "Bricklets" page
:: on www.daynix.com for more.
::
:: This code is licensed under standard 3-clause BSD license.
:: See file LICENSE supplied with this package for the full license text.
::
echo Enable network...
wmic path win32_networkadapter where name="Intel(R) PRO/1000 MT Network Connection" call enable
