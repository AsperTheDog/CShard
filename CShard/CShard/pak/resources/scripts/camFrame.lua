newPos = cam.pos

cam:move(Vec3:new(0, 0, -2 * getdt()))
obj = getObject(43)
trans = obj.transform
pos = trans.pos

cam:anchor(pos)