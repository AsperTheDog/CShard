li = obj:component(0)
li.color = Vec3.new(0, 0, 1)
trans = obj.transform
trans.pos = Vec3.new(3, 0, 2)
bouncing = false

fade = getPost(2)
grain = getPost(1)