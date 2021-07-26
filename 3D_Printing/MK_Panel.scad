// Pull this file in with use<>

include <BoxCommon.scad>



// innerPanel => the size of the panel at the origin of
// the outer radii

// panel=> the x/y/z size of the panel extents
// lip => z of lip above the panel
// RO,RL,RI => the outer most, middle, and inner most radius

module dressPanel(panel,lipHt, RO,RL,RI)
{
    innerPanel=[panel.x-RO*2, panel.y-RO*2,panel.z-1];
    
    // Virtually center the panel on the origin
    translate([-innerPanel.x/2,-innerPanel.y/2,0])
    {
        minkowski()
        {
            cube(innerPanel,center=false);
            cylinder(r=RO,h=1);
        }

        translate([0,0,panel.z])
        {
            difference()
            {
                minkowski()
                {
                    cube(innerPanel,center=false);
                    cylinder(r=RL,h=lipHt);
                }
                minkowski()
                {
                    cube(innerPanel,center=false);
                    cylinder(r=RI,h=lipHt+1);
                }
            }
        }
    }
}

module testDressPanel()
{   
    R=15;
    
    difference()
    {
        dressPanel([60,40,4],6,R,R-3,R-6);
        cube([100,100,20]);
    }
}
testDressPanel();