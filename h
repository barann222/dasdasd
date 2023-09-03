-- Set hitbox size, transparency level, and notification status
local size = Vector3.new(10, 10, 10)
local trans = 1
local notifications = false

local Circle = {}

-- StarterGui.ScreenGui
Circle["1"] = Instance.new("ScreenGui", game:GetService("Players").LocalPlayer:WaitForChild("PlayerGui"))
Circle["1"].ZIndexBehavior = Enum.ZIndexBehavior.Sibling

-- StarterGui.ScreenGui.Frame
Circle["2"] = Instance.new("ImageLabel", Circle["1"])
Circle["2"].BorderSizePixel = 0
Circle["2"].BackgroundColor3 = Color3.fromRGB(255, 255, 255)
Circle["2"].AnchorPoint = Vector2.new(1, 0)
Circle["2"].Image = "rbxassetid://4653037680"
Circle["2"].Size = UDim2.new(0, 486, 0, 466)
Circle["2"].BorderColor3 = Color3.fromRGB(0, 0, 0)
Circle["2"].Name = "Frame"
Circle["2"].BackgroundTransparency = 1
Circle["2"].Position = UDim2.new(0.626000047, 0, 0.25, 0)


-- Store the time when the code starts executing
local start = os.clock()

-- Send a notification saying that the script is loading
game.StarterGui:SetCore("SendNotification", {
    Title = "Script",
    Text = "Loading script...",
    Icon = "",
    Duration = 5
})

local red = Color3.fromRGB(255, 0, 4) 
local orange = Color3.fromRGB(252, 186, 3) 
local green = Color3.fromRGB(3, 252, 24) 
local blue = Color3.fromRGB(3, 3, 252) 

-- Load the ESP library and turn it on
local esp = loadstring(game:HttpGet("https://raw.githubusercontent.com/barann222/MAINHUBESP/main/asdasd"))()
esp:Toggle(true)

-- Configure ESP settings

-- Add an object listener to the workspace to detect enemy models
esp:AddObjectListener(workspace, {
    Name = "soldier_model",
    Type = "Model",
    Color = SelectedColor, -- Renk tanımlaması düzeltilmiş hali

    -- Specify the primary part of the model as the HumanoidRootPart
    PrimaryPart = function(obj)
        local root
        repeat
            root = obj:FindFirstChild("HumanoidRootPart")
            task.wait()
        until root
        return root
    end,

    -- Use a validator function to ensure that models do not have the "friendly_marker" child
    Validator = function(obj)
        task.wait(1)
        if obj:FindFirstChild("friendly_marker") then
            return false
        end
        return true
    end,



    -- Enable the ESP for enemy models
    IsEnabled = "enemy"
})

-- Enable the ESP for enemy models
esp.enemy = true

-- Wait for the game to load fully before applying hitboxes
task.wait(1)

-- Apply hitboxes to all existing enemy models in the workspace
for _, v in pairs(workspace:GetDescendants()) do
    if v.Name == "soldier_model" and v:IsA("Model") and not v:FindFirstChild("friendly_marker") then
        local pos = v:FindFirstChild("HumanoidRootPart").Position
        for _, bp in pairs(workspace:GetChildren()) do
            if bp:IsA("BasePart") then
                local distance = (bp.Position - pos).Magnitude
                if distance <= 5 then
                    bp.Transparency = trans
                    bp.Size = size
                    bp.Color = SelectedColor -- Box rengini seçilen renk ile güncelle
                end
            end
        end
    end
end

-- Function to handle when a new descendant is added to the workspace
local function handleDescendantAdded(descendant)
    task.wait(1)

    -- If the new descendant is an enemy model and notifications are enabled, send a notification
    if descendant.Name == "soldier_model" and descendant:IsA("Model") and not descendant:FindFirstChild("friendly_marker") then
        if notifications then
            game.StarterGui:SetCore("SendNotification", {
                Title = "Script",
                Text = "[Warning] New Enemy Spawned! Applied hitboxes.",
                Icon = "",
                Duration = 3
            })
        end

        -- Apply hitboxes to the new enemy model
        local pos = descendant:FindFirstChild("HumanoidRootPart").Position
        for _, bp in pairs(workspace:GetChildren()) do
            if bp:IsA("BasePart") then
                local distance = (bp.Position - pos).Magnitude
                if distance <= 5 then
                    bp.Transparency = trans
                    bp.Size = size
                    bp.Color = SelectedColor -- Box rengini seçilen renk ile güncelle
                end
            end
        end
    end
end

-- Connect the handleDescendantAdded function to the DescendantAdded event of the workspace
task.spawn(function()
    game.Workspace.DescendantAdded:Connect(handleDescendantAdded)
end)

-- Store the time when the code finishes executing
local finish = os.clock()

-- Calculate how long the code took to run and determine a rating for the loading speed
local time = finish - start
local rating
if time < 3 then
    rating = "fast"
elseif time < 5 then
    rating = "acceptable"
else
    rating = "slow"
end

-- Send a notification showing how long the code took to run and its rating
game.StarterGui:SetCore("SendNotification", {
    Title = "Script",
    Text = string.format("Script loaded in %.2f seconds (%s loading)", time, rating),
    Icon = "",
    Duration = 5
})

-- ESP'yi açma ve kapatma işlemi için düğme dinlemesi ekle
local espEnabled = true -- ESP başlangıçta açık olsun
game:GetService("UserInputService").InputBegan:Connect(function(input, isProcessed)
    if not isProcessed and input.KeyCode == Enum.KeyCode.T then -- "T" tuşuna bastığınız düşünülüyor
        espEnabled = not espEnabled -- ESP'yi açıkken kapatın veya kapalıyken açın
        esp:Toggle(espEnabled) -- ESP durumunu güncelleyin
        if espEnabled then
            game.StarterGui:SetCore("SendNotification", {
                Title = "ESP",
                Text = "ESP açıldı.",
                Icon = "",
                Duration = 3
            })
        else
            game.StarterGui:SetCore("SendNotification", {
                Title = "ESP",
                Text = "ESP kapandı.",
                Icon = "",
                Duration = 3
            })
        end
    end

    if FOV == false then
        Circle["2"]:Destroy()
    end

    -- ESP kapandığında Circle'ı kapatın
    if esp and not espEnabled then
        Circle["2"].Visible = false
    else
        Circle["2"].Visible = true
    end
local SelectedColor = red
end)
