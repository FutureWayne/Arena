// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/BlasterHUD.h"

void ABlasterHUD::DrawHUD()
{
	Super::DrawHUD();

	if (GEngine)
	{
		FVector2D ViewportSize;
		GEngine->GameViewport->GetViewportSize(ViewportSize);
		const FVector2D ViewportCenter(ViewportSize.X / 2.0f, ViewportSize.Y / 2.0f);

		float SpreadScaled = HUDPackage.CrosshairSpread * MaxCrosshairSpread;

		if (HUDPackage.CrosshairCenter.IsValid())
		{
			const FVector2D Spread(0.0f, 0.0f);
			DrawCrosshair(HUDPackage.CrosshairCenter.Get(), ViewportCenter, Spread);
		}

		if (HUDPackage.CrosshairLeft.IsValid())
		{
			const FVector2D Spread(-SpreadScaled, 0.0f);
			DrawCrosshair(HUDPackage.CrosshairLeft.Get(), FVector2D(ViewportCenter.X, ViewportCenter.Y), Spread);
		}

		if (HUDPackage.CrosshairRight.IsValid())
		{
			const FVector2D Spread(SpreadScaled, 0.0f);
			DrawCrosshair(HUDPackage.CrosshairRight.Get(), FVector2D(ViewportCenter.X, ViewportCenter.Y), Spread);
		}

		if (HUDPackage.CrosshairTop.IsValid())
		{
			const FVector2D Spread(0.0f, -SpreadScaled);
			DrawCrosshair(HUDPackage.CrosshairTop.Get(), FVector2D(ViewportCenter.X, ViewportCenter.Y), Spread);
		}

		if (HUDPackage.CrosshairBottom.IsValid())
		{
			const FVector2D Spread(0.0f, SpreadScaled);
			DrawCrosshair(HUDPackage.CrosshairBottom.Get(), FVector2D(ViewportCenter.X, ViewportCenter.Y), Spread);
		}
	}
}

void ABlasterHUD::DrawCrosshair(UTexture2D* Texture, const FVector2D ViewportCenter, const FVector2D Spread)
{
	const float TextureWidth = Texture->GetSizeX();
	const float TextureHeight = Texture->GetSizeY();

	const FVector2D TextureDrawPoint(
		ViewportCenter.X - TextureWidth / 2.0f + Spread.X,
		ViewportCenter.Y - TextureHeight / 2.0f + Spread.Y);
	
	DrawTexture(
		Texture,
		TextureDrawPoint.X,
		TextureDrawPoint.Y,
		TextureWidth,
		TextureHeight,
		0.0f,
		0.0f,
		1.0f,
		1.0f,
		FLinearColor::White);
}
