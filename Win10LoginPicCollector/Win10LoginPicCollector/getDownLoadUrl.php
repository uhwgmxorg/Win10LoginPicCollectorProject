<?php
/*
    This file is part of GUP.

    GUP is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    GUP is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with GUP.  If not, see <http://www.gnu.org/licenses/>.
*/

$lastestVersion = 1.0; // X.YZ
$lastestVersionStr = "1.0"; // X.Y.Z
$DLURL = "https://uhwgmxorg.com/Win10LoginPicCollector/Download/Setup_Win10LoginPicCollector_x32_1_1_0_0.exe";
$curentVersion = $_GET["version"];
$param = $_GET["param"]; // optional

if ($curentVersion >= $lastestVersion)
{
	echo 
"<?xml version=\"1.0\"?>
<GUP>
	<NeedToBeUpdated>no</NeedToBeUpdated>
</GUP>
";
}
else
{
	echo 
"<?xml version=\"1.0\"?>
<GUP>
	<NeedToBeUpdated>yes</NeedToBeUpdated>
	<Version>$lastestVersionStr</Version>
	<Location>$DLURL</Location>
</GUP>"
;
}
?>
