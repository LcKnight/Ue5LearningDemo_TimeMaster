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
		// �����ҽ�ɫ�Ƿ����
		if (ATimeMasterCharacter* TimeMasterCharacter = Cast<ATimeMasterCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0)))
		{
			TimeMasterCharacter->TimeReverseDelegate.AddDynamic(this, &UTimeReversalComponent::SetTimeReversing);
		}
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
		if (HeadFrame && TailFrame && HeadFrame == TailFrame) {
			RecordTimeLength = 0.f;
			IsOutdated = true;
		}
		else if (TailFrame) {
			// ֱ������λ�ú���ת����Ϊ����ģ���ѽ���
			Owner->SetActorLocation(TailFrame->GetValue().Location);
			Owner->SetActorRotation(TailFrame->GetValue().Rotation);

			// �Ƴ������ٶ����ã���Ϊ����ģ���ѽ���
			// USMC->SetPhysicsLinearVelocity(TailFrame->GetValue().LinearVelocity);
			// USMC->SetPhysicsAngularVelocityInDegrees(TailFrame->GetValue().AngularVelocity);

			RecordTimeLength -= TailFrame->GetValue().DeltaTime;
			TimeFrames.RemoveNode(TailFrame);
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
			// ���û��������ģ��
			if (TimeReversingState)
			{
				USMC->SetSimulatePhysics(false);
			}
			else
			{
				USMC->SetSimulatePhysics(true);
				// ��ʱ����ݽ���ʱ����������һ����ʼ�ٶȣ�ʹ��ӻ��ݵ㿪ʼ�����˶�
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

