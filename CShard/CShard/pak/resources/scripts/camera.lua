cam = obj:component(1)
cam:setActive()
initialPos = Vec3:new(14.900, 2.400, 26.300)

att1 = 1
att2 = 1
fade1 = getPost(0)
fade2 = getPost(3)
fade1.intensity = 0
fade2.intensity = 0
activateFog = false

obelist = getObject(43)
obTrans = obelist.transform
obPos = obTrans.pos

function clamp(x, min, max)
    if x < min then return min end
    if x > max then return max end
    return x
end