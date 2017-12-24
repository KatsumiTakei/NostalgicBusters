--[[
/**
 *	@file	EnemyMove.lua
 *	@date	2017 /11 / 25
 *	@author	Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */
]]
--//    !<  関数の数
MovePatternMax = 11

function MoveTest(argEnemy)
    
    argEnemy.spd = Vec2(0)
    
end

--[[
/**
 *	@fn		Pattern0
 *	@brief	下がってきて停滞して上がっていく
 */
]]
moveDescription0 = "下がってきて停滞して上がっていく"
function MovePattern0(argEnemy)
    
    if (argEnemy.count == 0) then
        argEnemy.spd.y = 3.0
	elseif (argEnemy.count == 40) then
		argEnemy.spd.y = 0.0
	elseif (argEnemy.count == 40 + argEnemy.data.wait) then
		argEnemy.spd.y = -3.0
	end

end
--[[
/**
 *	@fn	    Pattern1
 *	@brief  下がってきて停滞して左下に行く
 */
]]
moveDescription1 = "下がってきて停滞して左下に行く"
function MovePattern1(argEnemy)

	if (argEnemy.count == 0) then
		argEnemy.spd.y = 3.0
	elseif (argEnemy.count == 40) then
		argEnemy.spd.y = 0.0
	elseif (argEnemy.count == 40 + argEnemy.data.wait) then
		argEnemy.spd = Vec2(-1.0, 2.0)
    end
end
--[[
/**
 *	@fn		Pattern2
 *	@brief  下がってきて停滞して右下に行く
 */
]]
moveDescription2 = "下がってきて停滞して右下に行く"
function MovePattern2(argEnemy)
	
	if (argEnemy.count == 0) then
		argEnemy.spd.y = 3.0
	elseif (argEnemy.count == 40) then
		argEnemy.spd.y = 0.0
	elseif (argEnemy.count == 40 + argEnemy.data.wait) then
		argEnemy.spd = Vec2(1.0, 2.0)
    end
end
--[[
/**
 *	@fn		Pattern3
 *	@brief  すばやく降りてきて左へ
 */
]]
moveDescription3 = "すばやく降りてきて左へ"
function MovePattern3(argEnemy)

	if (argEnemy.count == 0) then
		argEnemy.spd.y = 5.0
	elseif (argEnemy.count < 100) then
		argEnemy.spd:sub(Vec2(0.05))
    end
	
end
--[[
/**
 *	@fn		Pattern4
 *	@brief	すばやく降りてきて右へ
 */
]]
moveDescription4 = "すばやく降りてきて右へ"
function MovePattern4(argEnemy)

	if (argEnemy.count == 0) then
		argEnemy.spd.y = 5.0
	elseif (argEnemy.count < 100) then
		argEnemy.spd:sub(Vec2(-0.05, 0.05))
    end
end
--[[
/**
 *	@fn		Pattern5
 *	@brief	斜め左下へ
 */
]]
moveDescription5 = "斜め左下へ"
function MovePattern5(argEnemy)

	if (argEnemy.count == 0) then
		argEnemy.spd:add(Vec2(-1.0, 2.0))
    end
end
--[[
/**
 *	@fn		Pattern6
 *	@brief	斜め右下へ
 */
]]
moveDescription6 = "斜め右下へ"
function MovePattern6(argEnemy)

	if (argEnemy.count == 0) then
		argEnemy.spd:add(Vec2(1.0, 2.0))
    end

end
--[[
/**
 *	@fn		Pattern7
 *	@brief	停滞してそのまま左上に
 */
]]
moveDescription7 = "停滞してそのまま左上に"
function MovePattern7(argEnemy)
	if (argEnemy.count == argEnemy.data.wait) then
		argEnemy.spd:sub(Vec2(0.7, 0.3))
	end
end
--[[
/**
 *	@fn		Pattern8
 *	@brief	停滞してそのまま右上に
 */
]]
moveDescription8 = "停滞してそのまま右上に"
function MovePattern8(argEnemy)
    if (argEnemy.count == argEnemy.data.wait) then
		argEnemy.spd:sub(Vec2(-0.7, 0.3))
	end
end
--[[
/**
 *	@fn		Pattern9
 *	@brief	停滞してそのまま上に
 */
]]
moveDescription9 = "停滞してそのまま上に"
function MovePattern9(argEnemy)
	if (argEnemy.count == argEnemy.data.wait) then
		argEnemy.spd.y = -1.0
    end
end
--[[
/**
 *	@fn		Pattern10
 *	@brief	下がってきてウロウロして上がっていく
 */
]]
moveDescription10 = "下がってきてウロウロして上がっていく"
function MovePattern10(argEnemy)
	if (argEnemy.count == 0) then
		argEnemy.spd.y = 4.0
	elseif (argEnemy.count == 40) then
		argEnemy.spd.x = -1.0
		argEnemy.spd.y = 0.0
	elseif (argEnemy.count >= 40) then
		if (argEnemy.count % 60 == 0) then
			local r = (math.cos(argEnemy.angle) < 0) and 0 or math.pi	--//	!<	cosの値が0より大きいならPI足す
			argEnemy.angle = math.rad(math.pi * 0.25 + r)
        end	
	end
	
	if(argEnemy.count % 120 == 0)then
		argEnemy.spd.x = -argEnemy.spd.x
	end
	if (argEnemy.count >= 40 + argEnemy.data.wait) then
		argEnemy.spd.y = argEnemy.spd.y - 0.05
		if(argEnemy.spd.y < -8)then
			argEnemy.spd.y = -8
		end
    end

end

function MovePattern11(argEnemy)
end

