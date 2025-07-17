# NetPro Multiplayer Game System

Demo Video: https://www.bilibili.com/video/BV1Yt3kzWEWM/?vd_source=c096d37a6e7624ca39a2afef5c3f64d2

A multiplayer game system built on Unreal Engine 4, integrating core gameplay modules including item collection, inventory management, and shop trading systems, demonstrating game development workflow from client to data management.

## Core Features

### Multiplayer Network Synchronization
- Real-time player state synchronization: Server RPC and Multicast RPC ensure game state synchronization
- Server authority validation: HasAuthority() prevents client-side cheating
- Network data replication: DOREPLIFETIME implements critical data synchronization

### Item Collection System
- Struct data-driven: FShopItemSkin structured item data
- JSON configuration: Local JSON files manage item properties
- Dynamic loading: Runtime loading of item information from JSON

### Inventory Management System
- Component-based design: PackageComponent independent inventory functionality
- UI interaction: UMG drag-and-drop system supports intuitive operations
- Real-time updates: Delegate system achieves UI-logic decoupling

### Shop Trading System
- Data-driven: JSON configuration for shop items and store settings
- Network synchronization: Purchase states synchronized across all clients
- Dynamic UI: Purchased items automatically hidden

## Technical Architecture

### Network Architecture

Server authority validation example:
```cpp
void ANetProGameState::AddScoreToPlayer(int32 PlayerID, int32 Score)
{
    if(!HasAuthority())
    {
        Server_AddScoreToPlayer(PlayerID, Score);
        return;
    }
    
    if(PlayerID == 1)
    {
        PlayerOneScore += Score;
        OnScoreChanged.Broadcast(1, PlayerOneScore);
    }
}
```

Dynamic Player ID Assignment:
```cpp
void ANetProGameMode::PostLogin(APlayerController* NewPlayer)
{
    Super::PostLogin(NewPlayer);
    int32 PlayerCount = GetNumPlayers();
    int32 AssignedPlayerID = PlayerCount;
    
    if(ANetProPlayerCharacter* PlayerChar = Cast<ANetProPlayerCharacter>(NewPlayer->GetPawn()))
    {
        PlayerChar->PlayerID = AssignedPlayerID;
    }
    else
    {
        // Delayed assignment to solve Pawn creation timing issue
        GetWorld()->GetTimerManager().SetTimerForNextTick([this, NewPlayer, AssignedPlayerID]()
        {
            if(ANetProPlayerCharacter* PlayerChar = Cast<ANetProPlayerCharacter>(NewPlayer->GetPawn()))
            {
                PlayerChar->PlayerID = AssignedPlayerID;
            }
        });
    }
}
```

Network Data Replication:
```cpp
void ANetProGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(ANetProGameState, PlayerOneScore);
    DOREPLIFETIME(ANetProGameState, PlayerTwoScore);
    DOREPLIFETIME(ANetProGameState, RemainingTime);
    DOREPLIFETIME(ANetProGameState, PurchasedItemsArray);
}
```

### Data Management System

JSON Data-Driven Shop System with runtime dynamic loading:
```cpp
void UShopItemSubsystem::LoadItemsFromJson()
{
    FString JsonPath = FPaths::ProjectContentDir() + TEXT("NetProject/Data/ItemData.json");
    FString JsonString;
    
    if(!FFileHelper::LoadFileToString(JsonString, *JsonPath))
    {
        UE_LOG(LogTemp, Log, TEXT("找不到Json文件%s"), *JsonPath);
        return;
    }
    
    TSharedRef<TJsonReader<TCHAR>> JsonReader = TJsonReaderFactory<TCHAR>::Create(JsonString);
    TArray<TSharedPtr<FJsonValue>> JsonArray;
    
    if(FJsonSerializer::Deserialize(JsonReader, JsonArray))
    {
        for(auto& Item : JsonArray)
        {
            TSharedPtr<FJsonObject> ItemObject = Item->AsObject();
            if(ItemObject.IsValid())
            {
                FShopItemSkin* NewSkin = new FShopItemSkin();
                NewSkin->ID = ItemObject->GetIntegerField(TEXT("ID"));
                NewSkin->Name = FText::FromString(ItemObject->GetStringField(TEXT("Name")));
                NewSkin->Price = ItemObject->GetIntegerField(TEXT("Price"));
                
                SkinMap.Add(NewSkin->ID, NewSkin);
            }
        }
    }
}
```

Purchase State Synchronization:
```cpp
void ANetProGameState::PurchaseItem(int32 ItemID)
{
    if(!HasAuthority())
    {
        return; // 只能在服务器上调用
    }
    
    if(!PurchasedItemsArray.Contains(ItemID))
    {
        PurchasedItemsArray.Add(ItemID);
        
        // 服务器购买成功后广播给所有客户端
        Multi_OnItemPurchased(ItemID);
        ForceNetUpdate();
    }
}
```

## Core Features

### 1. Stable Network Synchronization Mechanism
- Server RPC: Client requests server execution
- Multicast RPC: Server broadcasts to all clients
- Data Replication: Critical data automatic synchronization
- Authority Validation: HasAuthority() ensures game fairness

### 2. Component-Based Game Architecture
- PackageComponent: Independent inventory component
- ShopItemSubsystem: Item management subsystem
- UI Delegate System: Achieves UI-logic decoupling

### 3. Dynamic Spawn Point System
```cpp
AActor* ANetProGameMode::ChoosePlayerStart_Implementation(AController* Player)
{
    int32 PlayerCount = GetNumPlayers();
    TArray<AActor*> PlayerStarts;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ANetProPlayerStart::StaticClass(), PlayerStarts);
    
    for(AActor* Start : PlayerStarts)
    {
        if(ANetProPlayerStart* CustomStart = Cast<ANetProPlayerStart>(Start))
        {
            if(CustomStart->AssignedPlayerID == PlayerCount + 1)
            {
                return CustomStart;
            }
        }
    }
    return Super::ChoosePlayerStart_Implementation(Player);
}
```

### 4. Real-time Scoring System
- Server-side scoring: Ensures data accuracy
- Client UI updates: OnRep functions achieve real-time display
- Countdown mechanism: Timer system manages game time

## Technology Stack
- Engine: Unreal Engine 4
- Language: C++ + Blueprint
- Network: UE4 Built-in Network System
- Data: JSON + Struct Hybrid Architecture
- UI: UMG Interface System

## Project Highlights
1. Multiplayer game network architecture
2. JSON-driven data management system
3. Component-based modular design
4. Stable server-client synchronization mechanism
5. Dynamic player management system
6. Real-time UI data binding

---

# NetPro 联机游戏系统

观看视频：https://www.bilibili.com/video/BV1Yt3kzWEWM/?vd_source=c096d37a6e7624ca39a2afef5c3f64d2

一个基于虚幻引擎4的联机游戏系统，集成了道具收集、背包管理、商店交易等核心玩法模块，展示了从客户端到数据管理的游戏开发流程。

## 核心功能

### 联机网络同步
- 实时玩家状态同步：Server RPC 和 Multicast RPC 确保游戏状态同步
- 服务器权限验证：HasAuthority() 防止客户端作弊
- 网络数据复制：DOREPLIFETIME 实现关键数据同步

### 道具收集系统
- 结构体数据驱动：FShopItemSkin 结构化道具数据
- JSON配置化：本地JSON文件管理道具属性
- 动态加载：运行时从JSON加载道具信息

### 背包管理系统
- 组件化设计：PackageComponent 独立背包功能
- UI交互：UMG拖拽系统支持直观操作
- 实时更新：委托系统实现UI与逻辑解耦

### 商店交易系统
- 数据驱动：JSON配置商品信息和商店设置
- 网络同步：购买状态全客户端同步
- 动态UI：已购买商品自动隐藏

## 技术架构

### 网络架构
服务器权限验证示例：
```cpp
void ANetProGameState::AddScoreToPlayer(int32 PlayerID, int32 Score)
{
    if(!HasAuthority())
    {
        Server_AddScoreToPlayer(PlayerID, Score);
        return;
    }
    
    if(PlayerID == 1)
    {
        PlayerOneScore += Score;
        OnScoreChanged.Broadcast(1, PlayerOneScore);
    }
}
```

玩家ID动态分配：
```cpp
void ANetProGameMode::PostLogin(APlayerController* NewPlayer)
{
    Super::PostLogin(NewPlayer);
    int32 PlayerCount = GetNumPlayers();
    int32 AssignedPlayerID = PlayerCount;
    
    if(ANetProPlayerCharacter* PlayerChar = Cast<ANetProPlayerCharacter>(NewPlayer->GetPawn()))
    {
        PlayerChar->PlayerID = AssignedPlayerID;
    }
    else
    {
        // 延迟分配解决Pawn创建时机问题
        GetWorld()->GetTimerManager().SetTimerForNextTick([this, NewPlayer, AssignedPlayerID]()
        {
            if(ANetProPlayerCharacter* PlayerChar = Cast<ANetProPlayerCharacter>(NewPlayer->GetPawn()))
            {
                PlayerChar->PlayerID = AssignedPlayerID;
            }
        });
    }
}
```

网络数据复制：
```cpp
void ANetProGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(ANetProGameState, PlayerOneScore);
    DOREPLIFETIME(ANetProGameState, PlayerTwoScore);
    DOREPLIFETIME(ANetProGameState, RemainingTime);
    DOREPLIFETIME(ANetProGameState, PurchasedItemsArray);
}
```

### 数据管理系统

JSON数据驱动，商店系统采用JSON配置，支持运行时动态加载：
```cpp
void UShopItemSubsystem::LoadItemsFromJson()
{
    FString JsonPath = FPaths::ProjectContentDir() + TEXT("NetProject/Data/ItemData.json");
    FString JsonString;
    
    if(!FFileHelper::LoadFileToString(JsonString, *JsonPath))
    {
        UE_LOG(LogTemp, Log, TEXT("找不到Json文件%s"), *JsonPath);
        return;
    }
    
    TSharedRef<TJsonReader<TCHAR>> JsonReader = TJsonReaderFactory<TCHAR>::Create(JsonString);
    TArray<TSharedPtr<FJsonValue>> JsonArray;
    
    if(FJsonSerializer::Deserialize(JsonReader, JsonArray))
    {
        for(auto& Item : JsonArray)
        {
            TSharedPtr<FJsonObject> ItemObject = Item->AsObject();
            if(ItemObject.IsValid())
            {
                FShopItemSkin* NewSkin = new FShopItemSkin();
                NewSkin->ID = ItemObject->GetIntegerField(TEXT("ID"));
                NewSkin->Name = FText::FromString(ItemObject->GetStringField(TEXT("Name")));
                NewSkin->Price = ItemObject->GetIntegerField(TEXT("Price"));
                
                SkinMap.Add(NewSkin->ID, NewSkin);
            }
        }
    }
}
```

购买状态同步：
```cpp
void ANetProGameState::PurchaseItem(int32 ItemID)
{
    if(!HasAuthority())
    {
        return; // 只能在服务器上调用
    }
    
    if(!PurchasedItemsArray.Contains(ItemID))
    {
        PurchasedItemsArray.Add(ItemID);
        
        // 服务器购买成功后广播给所有客户端
        Multi_OnItemPurchased(ItemID);
        ForceNetUpdate();
    }
}
```

## 核心特性

### 1. 稳定的网络同步机制
- Server RPC：客户端请求服务器执行
- Multicast RPC：服务器广播给所有客户端
- 数据复制：关键数据自动同步
- 权限验证：HasAuthority() 确保游戏公平性

### 2. 组件化游戏架构
- PackageComponent：独立背包组件
- ShopItemSubsystem：道具管理子系统
- UI委托系统：实现UI与逻辑解耦

### 3. 动态出生点系统
```cpp
AActor* ANetProGameMode::ChoosePlayerStart_Implementation(AController* Player)
{
    int32 PlayerCount = GetNumPlayers();
    TArray<AActor*> PlayerStarts;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ANetProPlayerStart::StaticClass(), PlayerStarts);
    
    for(AActor* Start : PlayerStarts)
    {
        if(ANetProPlayerStart* CustomStart = Cast<ANetProPlayerStart>(Start))
        {
            if(CustomStart->AssignedPlayerID == PlayerCount + 1)
            {
                return CustomStart;
            }
        }
    }
    return Super::ChoosePlayerStart_Implementation(Player);
}
```

### 4. 实时计分系统
- 服务器端计分：确保数据准确性
- 客户端UI更新：OnRep函数实现实时显示
- 倒计时机制：Timer系统管理游戏时间

## 技术栈
- 引擎：虚幻引擎4
- 语言：C++ + 蓝图
- 网络：UE4内置网络系统
- 数据：JSON + 结构体混合架构
- UI：UMG界面系统

## 项目亮点
1. 多人游戏网络架构
2. JSON驱动的数据管理系统
3. 组件化的模块设计
4. 稳定的服务器-客户端同步机制
5. 动态的玩家管理系统
6. 实时的UI数据绑定
