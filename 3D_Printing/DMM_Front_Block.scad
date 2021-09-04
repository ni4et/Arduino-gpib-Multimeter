include <BoxCommon.scad>;
$fs=.01;
// This is going to look upside down because of
// coordinate system for the board is from the front.
// Whereas this view is from the back. Its all
// flipped on the x axis.


BlockSZ=[10,20,6+9]; //  Of the board holders
Front_Shift=[0,-CPU_Board_Loc.y-4,0];  // Center of DMM board 
DMM_HolderSZ=[100,23,6+9];

// Locate the clamp elements:
DMM_ClampScrewOffset=(DMM_HolderSZ.x+DMM_BOARD_X)/4;
ClampHole=5;

// Use these modules with the same translate parameters
module DMM_Front_U()
{  

    
   translate([5,-BlockSZ.y/2,BlockSZ.z/2]) cube(BlockSZ,center=true);
   translate([-25,-BlockSZ.y/2,BlockSZ.z/2]) cube(BlockSZ,center=true);
   
   translate([0,-7,DMM_HolderSZ.z/2] + Front_Shift) cube(DMM_HolderSZ,center=true);
}
module DMM_Front_D()
{
    translate(Front_Shift)
    {
       for (bl=boards) 
        {
                   // The board is centered on 0,0 and 7 above z
        #translate(bl+[-DMM_BOARD_X/2,-DMM_BOARD_Y/2,7])

            cube([DMM_BOARD_X, DMM_BOARD_Y,10]);
 
        }
        for (x=DMM_JAX_X)
        {
            translate([x,-DMM_JAX_Y,-.01])
                cylinder(h=20,d=DMM_JAX_LOWER_DIA);
            translate([x,-DMM_JAX_Y,DMM_JAX_Z_OFFS])
                cylinder(h=20,d=DMM_JAX_UPPER_DIA);
        }
        #translate([DMM_ClampScrewOffset,-DMM_JAX_Y,BlockSZ.z/2]) cylinder(h=20,d=ClampHole,center=false);
        #translate([-DMM_ClampScrewOffset,-DMM_JAX_Y,BlockSZ.z/2]) cylinder(h=20,d=ClampHole,center=false);
    }

    
}
difference()
{
    DMM_Front_U();
    DMM_Front_D();
}