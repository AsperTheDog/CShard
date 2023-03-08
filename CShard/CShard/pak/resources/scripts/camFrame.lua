newPos = cam.pos

cam:move(Vec3:new(0, 0, -2 * getdt()))
cam:anchor(obPos)