include <BoxCommon.scad>;
$fs=.01;
// This is going to look upside down because of
// coordinate system for the board is from the front.
// Whereas this view is from the back. Its all
// flipped on the x axis.

difference()
{
    // Base block
    translate([-70/2,-20,0]) cube([70,25,9]);
    // The board is centered on 0,0 and 7 above z
    translate([-DMM_BOARD_X/2,-DMM_BOARD_Y/2,7])
        cube([DMM_BOARD_X, DMM_BOARD_Y,8]);
    for (x=DMM_JAX_X)
    {
        translate([x,-DMM_JAX_Y,-.01])
            cylinder(h=20,d=DMM_JAX_LOWER_DIA);
        translate([x,-DMM_JAX_Y,DMM_JAX_Z_OFFS])
            cylinder(h=20,d=DMM_JAX_UPPER_DIA);
    }
}