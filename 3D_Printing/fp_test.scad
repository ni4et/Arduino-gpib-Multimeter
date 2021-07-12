include <BoxCommon.scad>
innerPanel=[DISPBLOCK.x,DISPBLOCK.y+10,1];


minkowski()
{
    cube(innerPanel,center=false);
    cylinder(r=10,h=1);
}

translate([0,0,2])
{
    difference()
    {
        minkowski()
        {
            cube(innerPanel,center=false);
            cylinder(r=7,h=5);
        }
        minkowski()
        {
            cube(innerPanel,center=false);
            cylinder(r=4,h=5.5);
        }
    }
};