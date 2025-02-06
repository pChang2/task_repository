#include "RotateActor.h"
#include "RotateChildActor.h"

ARotateActor::ARotateActor()
{
	PrimaryActorTick.bCanEverTick = true;

	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	SetRootComponent(SceneRoot);
	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComp"));
	StaticMeshComp->SetupAttachment(SceneRoot);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("/Game/rose1.rose1"));
	if (MeshAsset.Succeeded())
		StaticMeshComp->SetStaticMesh(MeshAsset.Object);
}

void ARotateActor::BeginPlay()
{
	Super::BeginPlay();

	float Angle = 360.0f / NumChildren;

	FActorSpawnParameters SpawnParams; // ���ο� ���͸� ������ �� ����� ���� ������ ��� ����ü
	SpawnParams.Owner = this; // ������ ������ �����ڸ� ���� ���ͷ� ���� -> �θ�� ����

	for (int32 i = 0; i < NumChildren; ++i)
	{
		float AngleInRadians = FMath::DegreesToRadians(i * Angle);

		FVector ChildLocation = GetActorLocation() +
			FVector(FMath::Cos(AngleInRadians) * Radius, FMath::Sin(AngleInRadians) * Radius, 0.0f);
		// �ڽ� ���Ͱ� ������ �ʱ� ��ġ ����
		FRotator ChildRotation = FRotator::ZeroRotator;
		// �ڽ� ������ �ʱ� ȸ��

		ARotateChildActor* ChildActor =
			GetWorld()->SpawnActor<ARotateChildActor>(ARotateChildActor::StaticClass(), ChildLocation, ChildRotation, SpawnParams);
		// GetWorld()->SpawnActor - ���忡 ���ο� ���͸� �����ϴ� �Լ� Instantiate
		// AChildActor -> ������ ������ Ŭ����
		// AChildActor::StaticClass() -> AChildActor Ŭ������ ��Ÿ �����͸� ��ȯ
		// ChildLocation -> ������ ������ �ʱ� ��ġ
		// ChildRotation -> ������ ������ �ʱ� ȸ��
		// SpawnParams -> ���� �� ����� ���� ����

		if (ChildActor)
			ChildActor->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);

		// �ڽ� ���Ͱ� ���� �ƴٸ�
		// AttachToActor -> ���� ���͸� �θ�� �����Ͽ� �ڽ� ���͸� ����
		// FAttachmentTransformRules::KeepWorldTransform -> �ڽ� ������ ���� ���� ��ġ�� ȸ���� �����ϸ� �θ� ����
	}
}

void ARotateActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AddActorLocalRotation(FRotator(0.0f, RotateSpeed * DeltaTime, 0.0f));
	// �θ� ȸ������ �ڽĵ��� ȸ����Ű�� ��
}
