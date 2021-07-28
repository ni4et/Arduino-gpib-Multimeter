include <BoxCommon.scad>;
$fs=.01;
// This is going to look upside down because of
// coordinate system for the board is from the front.
// Whereas this view is from the back. Its all
// flipped on the x axis.

Front_Shift=[0,-CPU_Board_Loc.y-4,0];

// Use these modules with the same translate parameters
module DMM_Front_U()
{  
    sz=[10,20,6+9];
    
   translate([5,-sz.y/2,sz.z/2]) cube(sz,center=true);
   translate([-25,-sz.y/2,sz.z/2]) cube(sz,center=true);
   
   translate([0,-7,9/2] + Front_Shift) cube([70,23,9],center=true);
}
module DMM_Front_D()
{
    translate(Front_Shift)
    {
       for (bl=boards) 
        {
                   // The board is centered on 0,0 and 7 above z
        #translate(bl+[-DMM_BOARD_X/2,-DMM_BOARD_Y/2,7])

            cube([DMM_BOARD_X, DMM_BOARD_Y,8]);
 
        }
        for (x=DMM_JAX_X)
        {
            translate([x,-DMM_JAX_Y,-.01])
                cylinder(h=20,d=DMM_JAX_LOWER_DIA);
            translate([x,-DMM_JAX_Y,DMM_JAX_Z_OFFS])
                cylinder(h=20,d=DMM_JAX_UPPER_DIA);
        }
    }
}
difference()
{
    DMM_Front_U();
    DMM_Front_D();
}