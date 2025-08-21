// Fill out your copyright notice in the Description page of Project Settings.


#include "TimeReversalComponent.h"
#include "TimeMasterCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/Actor.h"

// Sets default values for this component's properties
UTimeReversalComponent::UTimeReversalComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	Owner = nullptr;
	IsInit = false;
	IsTimeReversing = false;
	IsOutdated = false;
	RecordTimeLength = 0.0f;
	ReversalSpeed = 2.0;

	//即默认绑定委托
	bBind = true;
	// ...
}


// Called when the game starts
void UTimeReversalComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	if (!IsInit) {
		Owner = GetOwner();
		IsInit = true;
		// 检查玩家角色是否存在
		if (ATimeMasterCharacter* TimeMasterCharacter = Cast<ATimeMasterCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0)))
		{
			TimeMasterCharacter->TimeReverseDelegate.AddDynamic(this, &UTimeReversalComponent::SetTimeReversing);
		}
	}

}

void UTimeReversalComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	// 确保在组件销毁前，移除委托的绑定
	if (ATimeMasterCharacter* TimeMasterCharacter = Cast<ATimeMasterCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0)))
	{
		TimeMasterCharacter->TimeReverseDelegate.RemoveDynamic(this, &UTimeReversalComponent::SetTimeReversing);
	}
}


// Called every frame
void UTimeReversalComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
	if (!IsInit) {
		return;
	}
	if (!IsTimeReversing) {
		TArray<UStaticMeshComponent*> CompArray;

		// Then, call GetComponents<T>() on the owner actor.
		Owner->GetComponents<UStaticMeshComponent>(CompArray);
		if (CompArray.Num() > 0) {
			UStaticMeshComponent* USMC = Cast<UStaticMeshComponent>(CompArray[0]);
			if (USMC) {
				TimeInfo NewFrame(Owner->GetActorLocation(), Owner->GetActorRotation(), USMC->GetPhysicsLinearVelocity(), USMC->GetPhysicsAngularVelocityInDegrees(), DeltaTime);
				if (RecordTimeLength <= 15.f)
				{
					TimeFrames.AddTail(NewFrame);
					RecordTimeLength += DeltaTime;
					IsOutdated = false;
				}
				else
				{
					auto HeadFrame = TimeFrames.GetHead();
					float HeadDeltaTime = HeadFrame->GetValue().DeltaTime;
					TimeFrames.RemoveNode(HeadFrame);
					RecordTimeLength -= HeadDeltaTime;

					TimeFrames.AddTail(NewFrame);
					RecordTimeLength += DeltaTime;
					IsOutdated = false;
				}
			}
		}

	}
	else if (!IsOutdated)
	{

		auto HeadFrame = TimeFrames.GetHead();
		auto TailFrame = TimeFrames.GetTail();
		if (TimeFrames.Num() <=0) {
			RecordTimeLength = 0.f;
			IsOutdated = true;
		}
		else if (TailFrame) {
			if (TimeFrames.Num() > ReversalSpeed) 
			{
				auto TargetFrame = TailFrame;
				for (int i = 0; i < ReversalSpeed; i++) {
					TargetFrame = TailFrame->GetPrevNode();
				}

				// 直接设置位置和旋转，因为物理模拟已禁用
				Owner->SetActorLocation(TargetFrame->GetValue().Location);
				Owner->SetActorRotation(TargetFrame->GetValue().Rotation);

				// 移除物理速度设置，因为物理模拟已禁用
				// USMC->SetPhysicsLinearVelocity(TailFrame->GetValue().LinearVelocity);
				// USMC->SetPhysicsAngularVelocityInDegrees(TailFrame->GetValue().AngularVelocity);

				for (int i = 0; i < ReversalSpeed; i++) {
					auto NewTailFrame = TimeFrames.GetTail();
					RecordTimeLength -= NewTailFrame->GetValue().DeltaTime;
					TimeFrames.RemoveNode(NewTailFrame);
				}
			}
			else {
				// 直接设置位置和旋转，因为物理模拟已禁用
				Owner->SetActorLocation(HeadFrame->GetValue().Location);
				Owner->SetActorRotation(HeadFrame->GetValue().Rotation);
				TimeFrames.Empty();
				RecordTimeLength = 0.f;
				IsOutdated = true;

			}
		}
	}
}

void UTimeReversalComponent::SetTimeReversing(bool TimeReversingState)
{
	IsTimeReversing = TimeReversingState;

	TArray<UStaticMeshComponent*> CompArray;
	Owner->GetComponents<UStaticMeshComponent>(CompArray);
	if (CompArray.Num() > 0)
	{
		UStaticMeshComponent* USMC = CompArray[0];
		if (USMC)
		{
			// 启用或禁用物理模拟
			if (TimeReversingState)
			{
				USMC->SetSimulatePhysics(false);
			}
			else
			{
				USMC->SetSimulatePhysics(true);
				// 当时间回溯结束时，可以设置一个初始速度，使其从回溯点开始继续运动
				if (TimeFrames.Num() > 0)
				{
					const TimeInfo& LastFrame = TimeFrames.GetHead()->GetValue();
					USMC->SetPhysicsLinearVelocity(LastFrame.LinearVelocity);
					USMC->SetPhysicsAngularVelocityInDegrees(LastFrame.AngularVelocity);
				}
			}
		}
	}
}

void UTimeReversalComponent::ToggleDelegateBinding()
{
	if (ATimeMasterCharacter* TimeMasterCharacter = Cast<ATimeMasterCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0)))
	{
		if (bBind)
		{
			// 如果bBind为true，则解除委托绑定
			TimeMasterCharacter->TimeReverseDelegate.RemoveDynamic(this, &UTimeReversalComponent::SetTimeReversing);
			UE_LOG(LogTemp, Warning, TEXT("TimeReversalComponent: Delegate unbound."));
			bBind = false;
		}
		else
		{
			// 如果bBind为false，则添加委托绑定
			TimeMasterCharacter->TimeReverseDelegate.AddDynamic(this, &UTimeReversalComponent::SetTimeReversing);
			UE_LOG(LogTemp, Warning, TEXT("TimeReversalComponent: Delegate bound."));
			bBind = true;
		}
	}
}

