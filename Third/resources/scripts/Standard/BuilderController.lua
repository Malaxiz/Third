-- BuilderController.lua

local BuilderController = class (
    function(self, parent, component)
        self.parent = parent
        self.component = component

        self.mouseDown = false
        self.rightMouseDown = false
        self.initMX = 0
        self.initMY = 0
        self.mX = 0
        self.mY = 0
        
        self.hasSprite = false
        self.spriteKey = ""
        self.r = 255
        self.g = 0
        self.b = 0
        self.a = 255
                       
        self.activeEntity = nil
        self.editValues = false
                                 
        self.isBuilding = false
        self.isMoving = false
        self.isTiling = false

        self.isTileArea = false
        self.pos1 = nil
        self.pos2 = nil
                                 
        self.tileset = ""

        P = Position
        self.brushes = {
            {P(0, 0)},
            {P(0, 0), P(0, -1), P(-1, 0), P(1, 0), P(0, 1)},
            {P(0, 0), P(0, -1), P(-1, 0), P(1, 0), P(0, 1), P(1, 1), P(-1, -1), P(1, -1), P(-1, 1)},
            {P(0, 0), P(-1, -1), P(1, 1), P(-2, -2), P(2, 2)}
        }
        self.brush = 2
    end
)

function BuilderController:onComponentAdd(comp)
    if(comp == "Standard/ChatController") then
        chatController = self.parent:getComponent(comp)
        chatController:registerCommand("build", self)
        chatController:registerCommand("place", self)
        chatController:registerCommand("move", self)
        chatController:registerCommand("remove", self)
        chatController:registerCommand("color", self)

        chatController:registerCommand("tile", self)
        chatController:registerCommand("tilearea", self)
        chatController:registerCommand("brush", self)
    end
end

function BuilderController:createActiveEntity()
    if(self.hasSprite) then
        self.activeEntity = self.parent.entityManager:createSpriteEntity(Box(self.mX, self.mY, 0, 0), self.spriteKey)
    else
        self.activeEntity = self.parent.entityManager:createColoredEntity(Box(self.mX, self.mY, 0, 0), Color(self.r, self.g, self.b, self.a))
    end
       
    self.parent.entityManager:addEntity(self.activeEntity, "")
end

function BuilderController:build(commands)
    self.isBuilding = true
end

function BuilderController:place(commands)
    if(commands[2] and commands[3]) then
        self.isMoving = true
        self:createActiveEntity()
        self.activeEntity.body:setDimension(tonumber(commands[2]), tonumber(commands[3]))
        
        flagsToToggle = BitOR(EntityProperty.COLLIDABLE, EntityProperty.GRAVITY_AFFECT)
        self.activeEntity:removeProperty(flagsToToggle)
    end
end

function BuilderController:move(commands)
    self.activeEntity = self.parent.entityManager:getEntityAtCoordinate(self.mX, self.mY, nil)
    if(not self.activeEntity) then do return end end

    self.activeEntity:removeProperty(BitOR(EntityProperty.COLLIDABLE, EntityProperty.GRAVITY_AFFECT))
    self.isMoving = true
end

function BuilderController:remove(commands)
    self.activeEntity = self.parent.entityManager:getEntityAtCoordinate(self.mX, self.mY, nil)
    if(not self.activeEntity) then do return end end

    self.activeEntity.isDead = true
    self.activeEntity = nil
end

function BuilderController:color(commands)
    if(commands[3] == nil) then
        self.spriteKey = commands[2]
        self.hasSprite = true
    else
        self.r = tonumber(commands[2])
        self.g = tonumber(commands[3])
        self.b = tonumber(commands[4])
        if(commands[5]) then
            self.a = tonumber(commands[5])
        end
        self.hasSprite = false
    end
end

function BuilderController:tile(commands)

    if(not commands[2]) then
        self.isTiling = false
        do return end
    end

    self.tileset = commands[2]
    self.isTiling = true
end

function BuilderController:tilearea(commands)
    if(self.isTileArea) then
        self.isTileArea = false
        do return end
    end

    if(self.tileset == "") then
        do return end
    end

    self.isTileArea = true
    self.pos1 = Position(self.mX, self.mY)

end

function BuilderController:changeBrush(commands)
    if(not commands[2]) then do return end end

    self.brush = tonumber(commands[2])
end

function BuilderController:onChatCommand(commands)
    if(commands[1] == "build") then     self:build(commands) end
    if(commands[1] == "place") then     self:place(commands) end
    if(commands[1] == "move") then      self:move(commands) end
    if(commands[1] == "remove") then    self:remove(commands) end
    if(commands[1] == "color") then     self:color(commands) end
    if(commands[1] == "tile") then      self:tile(commands) end
    if(commands[1] == "tilearea") then  self:tilearea(commands) end
    if(commands[1] == "brush") then     self:changeBrush(commands) end
end

function BuilderController:onLoop()
    if(self.isBuilding) then
        if(self.mouseDown and self.activeEntity == nil) then
            self.initMX = self.mX
            self.initMY = self.mY
            self:createActiveEntity()
            
            flagsToToggle = BitOR(EntityProperty.COLLIDABLE, EntityProperty.GRAVITY_AFFECT)
            self.activeEntity:removeProperty(flagsToToggle)
        end

        if(not self.mouseDown and self.activeEntity ~= nil) then
            flagsToToggle = EntityProperty.COLLIDABLE
            self.activeEntity:addProperty(flagsToToggle)
            
            self.activeEntity = nil
            self.editValues = true
            self.isBuilding = false
            --self.component.instance.game.ignoreEvents = true
        end

        if(self.activeEntity ~= nil) then
            body = self.activeEntity.body
            
            dX = self.mX - self.initMX
            dY = self.mY - self.initMY
            self.activeEntity.body:setDimension(math.abs(dX), math.abs(dY))
            
            if(dX < 0) then
                body:setPosition(self.mX, body.box.y)
            end
            if(dY < 0) then
                body:setPosition(body.box.x, self.mY)
            end
        end
    end

    if(self.isMoving) then
        if(self.activeEntity ~= nil) then
            self.activeEntity.body:setPosition(self.mX, self.mY)
        else
            self.isMoving = false
        end
        
        if(self.mouseDown) then
            self.isMoving = false
            
            self.activeEntity:addProperty(EntityProperty.COLLIDABLE)
            self.activeEntity = nil
        end
    end

    if(self.isTiling) then
        tileSize = game.tileSize()

        if(self.mouseDown) then
            for k, v in pairs(self.brushes[self.brush]) do
                self.parent.entityManager:addTile(self.mX + v.x * tileSize, self.mY + v.y * tileSize, self.tileset)
            end
        end
        if(self.rightMouseDown) then
            for k, v in pairs(self.brushes[self.brush]) do
                self.parent.entityManager:removeTile(self.mX + v.x * tileSize, self.mY + v.y * tileSize)
            end
        end
    end

    if(self.isTileArea) then
        if(self.mouseDown) then
            self.isTileArea = false
            self.pos2 = Position(self.mX, self.mY)

            tileSize = game.tileSize()

            if(self.pos1.x > self.pos2.x) then
                temp = self.pos1.x
                self.pos1.x = self.pos2.x
                self.pos2.x = temp
            end
            if(self.pos1.y > self.pos2.y) then
                temp = self.pos1.y
                self.pos1.y = self.pos2.y
                self.pos2.y = temp
            end

            for y = self.pos1.y, self.pos2.y, tileSize do
                for x = self.pos1.x, self.pos2.x, tileSize do
                    self.parent.entityManager:addTile(x, y, self.tileset)
                end
            end

            self.pos1 = nil
            self.pos2 = nil
        end
    end
end

function BuilderController:onKeyStates(state)
    self.mX, self.mY = self.component:getRelativeMouse()
    self.mouseDown = game:leftMousePressed()
    self.rightMouseDown = game:rightMousePressed()
end

function BuilderController:onRenderAdditional()
    if(self.isTiling) then
        tileSize = game.tileSize()
        colorR = 0
        colorG = 255
        if(self.rightMouseDown) then
            colorR = 255
            colorG = 0
        end
        for k, v in pairs(self.brushes[self.brush]) do
            self.component:renderRect(self.mX - self.component.camera:offsetX() + v.x * tileSize - tileSize / 2, self.mY - self.component.camera:offsetY() + v.y * tileSize - tileSize / 2, tileSize, tileSize, colorR, colorG, 0, 100)
        end
    end
    if(self.isTileArea) then
        self.component:renderRect(self.pos1.x - self.component.camera:offsetX(), self.pos1.y - self.component.camera:offsetY(), self.mX - self.pos1.x, self.mY - self.pos1.y, 255, 255, 0, 100)
    end
end

function create(parent, component)
    return BuilderController(parent, component)
end