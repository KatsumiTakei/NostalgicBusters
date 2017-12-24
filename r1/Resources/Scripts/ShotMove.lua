--[[
/**
 *	@file	ShotMove.lua
 *	@date	2017 /11 / 25
 *	@author	Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */
]]
--//    !<  関数の数
ShotPatternMax = 17

--//    !<  C側の変数
PlayScreenLeft = 0
PlayScreenRight = 0

RandomSeed = os.time()
math.randomseed(RandomSeed)

shotTestDescription = "ばらまきショット"
function ShotTest(argBullet)
end

--[[
/**
 * @fn Rang
 */
]]
function Rang(argSeed)
    return -argSeed + argSeed * 2 * math.random(100 * 100) / 10000.0
end

--[[
/**
 * @fn HomingShot
 */
]]
function HomingShot(argBullet)
    return math.atan2(argBullet:Target().y - argBullet.enemy:Pos().y, argBullet:Target().x - argBullet.enemy:Pos().x)
end

--[[
/**
 * @fn RemoveShot
 */
]]
function RemoveShot(argBullet)
    local RemoveValue = 24.0
    return math.atan2(
        (argBullet:Target().y + RemoveValue) - argBullet.enemy:Pos().y,
        (argBullet:Target().x + RemoveValue) - argBullet.enemy:Pos().x
    )
end

--[[
/**
 * @fn ShotPattern0
 */
]]
shotDescription0 = "自機に向かって<直線>"
function ShotPattern0(argBullet)
    if (argBullet.count ~= 0) then
        return
    end

    argBullet.spd = Vec2(4.0)
    argBullet.pos = argBullet.enemy:Pos()
    argBullet.angle = HomingShot(argBullet)
end

--[[
/**
 * @fn ShotPattern1
 */
]]
shotDescription1 = "円形 + 60フレーム中に1発、自機に向かって<直線>"
function ShotPattern1(argBullet)
    if (argBullet.count ~= 0) then
        return
    end

    argBullet.pos = argBullet.enemy:Pos()
    argBullet.spd = Vec2(4.0)
    local target = HomingShot(argBullet)

    if (argBullet.enemy.count % 60 == 0) then
        argBullet.angle = HomingShot(argBullet)
    else
        argBullet.angle = (target + math.pi * argBullet.interval * 0.1) * 2.0
        local BulletMax = 10
        if (argBullet.interval < BulletMax) then
            argBullet.enemy:ShotBullet(argBullet.interval + 1)
        end
    end
end
--[[
/**
 * @fn ShotPattern2
 */
]]
shotDescription2 = "自機に向かってスピード変化<円形>(加速)"
function ShotPattern2(argBullet)
    if (argBullet.count == 0) then
        argBullet.pos = argBullet.enemy:Pos()
        local target = HomingShot(argBullet)

        argBullet.angle = (target + math.pi * argBullet.interval * 0.1) * 2.0

        local BulletMax = 10
        if (argBullet.interval < BulletMax) then
            argBullet.enemy:ShotBullet(argBullet.interval + 1)
        end

        return
    end

    local spd = 0.1 * argBullet.count
    argBullet.spd = Vec2(spd)
end
--[[
/**
 * @fn ShotPattern3
 */
]]
shotDescription3 = "自機に向かってスピード変化<直線>(減速)"
function ShotPattern3(argBullet)
    if (argBullet.count == 0) then
        argBullet.angle = HomingShot(argBullet)
        argBullet.pos = argBullet.enemy:Pos()
    end

    local spd = 5.0 - 0.05 * argBullet.count

    if (spd <= 2) then
        spd = 2
    end

    argBullet.spd = Vec2(spd)
end
--[[
/**
 * @fn ShotPattern4
 */
]]
shotDescription4 = "徐々に円形発射"
function ShotPattern4(argBullet)
    if (argBullet.count ~= 0) then
        return
    end

    local rot = HomingShot(argBullet)

    argBullet.angle = (rot + math.pi * math.sin(argBullet.enemy:Count())) * 20

    argBullet.angle = math.rad(argBullet.angle)

    if (argBullet.angle > 180) then
        argBullet.angle = 360 - argBullet.angle
    end
    argBullet.pos = argBullet.enemy:Pos()
    argBullet.spd = Vec2(4.0)
end

--[[
/**
 * @fn ShotPattern5
 */
]]
shotDescription5 = "ばらまきショット"
function ShotPattern5(argBullet)
    if (argBullet.count ~= 0) then
        return
    end

    local rand = math.pi * 0.25
    argBullet.angle = (HomingShot(argBullet) + Rang(rand))
    argBullet.pos = argBullet.enemy:Pos()
    argBullet.spd = Vec2(4 + Rang(2))
end
--[[
/**
 * @fn ShotPattern6
 */
]]
shotDescription6 = "ばらまきショット(減速)"
function ShotPattern6(argBullet)
    if (argBullet.count ~= 0) then
        if (argBullet.spd.x > 1.5) then
            argBullet.spd.x = argBullet.spd.x - 0.04
        end
        if (argBullet.spd.y > 1.5) then
            argBullet.spd.y = argBullet.spd.y - 0.04
        end
        return
    end

    local rand = math.pi * 0.25
    argBullet.angle = (HomingShot(argBullet) + Rang(rand))
    argBullet.pos = argBullet.enemy:Pos()
    argBullet.spd = Vec2(4 + Rang(2))
end
--[[
/**
 * @fn ShotPattern7
 */
]]
shotDescription7 = "バラ降らし"
function ShotPattern7(argBullet)
    if (argBullet.count == 0) then
        range = 0
        argBullet.angle = math.rad(90.0)
        argBullet.pos.x = argBullet.enemy:Pos().x + math.cos(range) * 100.0
        argBullet.pos.y = argBullet.enemy:Pos().y - math.sin(math.sin(range) * 100.0) * 20.0 + 20.0
        argBullet.spd = Vec2(0)
        range = range + 1
        if (range > 20) then
            range = 0
        end
    else
        if (argBullet.count == 5) then
            argBullet.enemy:ShotBullet()
        end
        if (argBullet.count == 60) then
            localeSpd = 1.0
            argBullet.spd = Vec2(math.sin(localeSpd) + 1.0, math.sin(localeSpd) + 1.0)
            localeSpd = localSpd + 1
            if (localeSpd > 4.0) then
                localeSpd = 1.0
            end
        end
    end
end
--[[
/**
 * @fn ShotPattern8
 */
]]
shotDescription8 = "左オプション"
function ShotPattern8(argBullet)
    if (argBullet.count ~= 0) then
        return
    end

    range = 0
    argBullet.angle = -(math.pi * 0.1 * range)
    argBullet.pos = argBullet.enemy:Pos() - Vec2(100.0)
    argBullet.spd = Vec2(2.0)
    range = range + 1
    if (range > 20) then
        range = 0
    else
        argBullet.enemy:ShotBullet()
    end
end
--[[
/**
 * @fn ShotPattern9
 */
]]
shotDescription9 = "米粒クロス右"
function ShotPattern9(argBullet)
    if (argBullet.count == 0) then
        rot = 10
        argBullet.angle = math.rad(math.pi * 2 / 60.0) + math.rad(rot)
        argBullet.spd = Vec2(2.0)
        rot = rot + 10

        if (rot > 360) then
            rot = 10
        else
            argBullet.enemy:ShotBullet()
        end
    else
        argBullet.angle = argBullet.angle - math.rad(30.0) / 60.0
    end
end
--[[
/**
 * @fn ShotPattern10
  */
]]
shotDescription10 = "米粒クロス左"
function ShotPattern10(argBullet)
    if (argBullet.count == 0) then
        rot = 10
        argBullet.angle = math.rad(math.pi * 2 / 60.0) + math.rad(rot)

        argBullet.spd = Vec2(2.0)
        rot = rot + 10

        if (rot > 360) then
            rot = 10
        else
            argBullet.enemy:ShotBullet()
        end
    else
        argBullet.angle = argBullet.angle + math.rad(30.0) / 60.0
    end
end
--[[
/**
 * @fn ShotPattern11
 */
]]
shotDescription11 = "輪っかホーミング"
function ShotPattern11(argBullet)
    if (argBullet.count ~= 0) then
        return
    end

    --// 弾幕の発射範囲
    rot = 10
    --// 範囲の計算
    argBullet.pos.x = argBullet.enemy:Pos().x + math.cos(math.rad(rot)) * 100.0
    argBullet.pos.y = argBullet.enemy:Pos().y + math.sin(math.rad(rot)) * 100.0
    argBullet.angle = HomingShot(argBullet)
    argBullet.spd = Vec2(1.0)
    rot = rot + 10
    if (rot > 360) then
        rot = 10
    else
        argBullet.enemy:ShotBullet(argBullet.pos)
    end
end
--[[
/**
 * @fn ShotPattern12
 */
]]
shotDescription12 = "円形ばらまき"
function ShotPattern12(argBullet)
    if (argBullet.count ~= 0) then
        return
    end

    rot = HomingShot(argBullet)
    range = 0

    argBullet.angle = (rot + math.pi * range)

    argBullet.spd = Vec2(2.0)
    range = range + 1

    if (range > 20) then
        range = 0
    else
        argBullet.enemy:ShotBullet()
    end
    argBullet.angle = argBullet.angle * 2
end
--[[
/**
 * @fn ShotPattern13
 */
]]
shotDescription13 = "菱形反射"
function ShotPattern13(argBullet)
    if (argBullet.count == 0) then
        range = 0

        argBullet.angle = math.rad(45.0)
        argBullet.pos.x = argBullet.enemy:Pos().x
        argBullet.pos.y = argBullet.enemy:Pos().y + range * 10.0
        argBullet.spd = Vec2(4.0)
        range = range + 1

        if (range > 3) then
            range = 0
        else
            argBullet.enemy:ShotBullet()
        end
    else
        if (argBullet.pos.x - argBullet.spd.x < PlayScreenLeft) then
            argBullet.angle = argBullet.angle + math.rad(90.0)
        elseif (argBullet.pos.x + argBullet.spd.x > PlayScreenRight) then
            argBullet.angle = argBullet.angle + math.rad(-90.0)
        end
    end
end
--[[
/**
 * @fn ShotPattern14
 */
]]
shotDescription14 = "菱形反射"
function ShotPattern14(argBullet)
    if (argBullet.count == 0) then
        argBullet.angle = math.rad(135.0)
        argBullet.pos.x = argBullet.enemy:Pos().x
        argBullet.pos.y = argBullet.enemy:Pos().y + argBullet.interval * 10.0
        argBullet.spd = Vec2(4.0)

        if (argBullet.interval > 3) then
            argBullet.interval = 0
        else
            argBullet.enemy:ShotBullet(argBullet.interval + 1)
        end
    else
        if (argBullet.pos.x - argBullet.spd.x < PlayScreenLeft) then
            argBullet.angle = argBullet.angle + math.rad(90.0)
        elseif (argBullet.pos.x + argBullet.spd.x > PlayScreenRight) then
            argBullet.angle = argBullet.angle + math.rad(-90.0)
        end
    end
end

--[[
/**
 * @fn ShotPattern15
 */
]]
shotDescription15 = "円形放射"
function ShotPattern15(argBullet)
    if (argBullet.count ~= 0) then
        argBullet.interval = 0
        return
    end

    local rot = HomingShot(argBullet)

    argBullet.angle = (rot + math.pi * argBullet.interval * 0.1) * 2.0
    argBullet.pos = argBullet.enemy:Pos()
    argBullet.spd = Vec2(4.0)

    local BulletMax = 10
    if (argBullet.interval < BulletMax) then
        argBullet.enemy:ShotBullet(argBullet.interval + 1)
    end
end

--[[
/**
 * @fn ShotPattern16
 */
]]
shotDescription16 = "自機狙い<扇型>"
function ShotPattern16(argBullet)
    if (argBullet.count ~= 0) then
        local spd = 0.05 * argBullet.count
        local StopTime = 60 - argBullet.angle
        local StartTime = 120 - argBullet.angle

        if (argBullet.count > StopTime and argBullet.count < StartTime) then
            spd = 0.01 * (StartTime - argBullet.count)
            if (spd < 0) then
                spd = 0
            end
        end

        if (argBullet.count > StartTime) then
            spd = 0.1 * (argBullet.count - StopTime)
        end

        argBullet.spd = Vec2(spd)

        return
    end

    argBullet.pos = argBullet.enemy:Pos()
    local target = HomingShot(argBullet)

    local BulletInterval = 0.05
    argBullet.angle = (target + math.pi * argBullet.interval * BulletInterval) - math.rad(90)

    local BulletMax = 20
    if (argBullet.interval < BulletMax) then
        argBullet.enemy:ShotBullet(argBullet.interval + 1)
    end
end
