
// N or t -- North
// E or r --  east
// W or l -- West
// S or b --south
// C center left to right
// M center top to bottom 

SS="";

if (search(SS,"nNtT"))
{
    echo("North");
}
if (search(SS,"eErR"))
{
    echo("East");
}
if (search(SS,"wWlL"))
{
    echo("West");
}
if (search(SS,"sSbB"))
{
    echo("South");
}
function align(mode,here,center=true)=
    [ search(mode,"eErR")?here.x:(search(mode,"wWlL")?0:here.x/2),
    search(mode,"nN")?here.y:(search(mode,"sS")?0:here.y/2),
    search(mode,"tT")?here.z:(search(mode,"bB")?0:here.z/2)]-
    (center?[here.x/2,here.y/2, here.z/2]:[0,0,0]);

echo("-",align("-",[2,3,6],center=false));
echo("N",align("N",[2,3,6],center=false));
echo("E",align("E",[2,3,6],center=false));
echo("S",align("S",[2,3,6],center=false));
echo("W",align("W",[2,3,6],center=false));
echo("T",align("T",[2,3,6],center=false));
echo("B",align("B",[2,3,6],center=false));
echo("Center on:");
echo("-",align("-",[2,3,6],center=true));
echo("N",align("N",[2,3,6],center=true));
echo("E",align("E",[2,3,6],center=true));
echo("S",align("S",[2,3,6],center=true));
echo("W",align("W",[2,3,6],center=true));
echo("T",align("T",[2,3,6],center=true));
echo("B",align("B",[2,3,6],center=true));
