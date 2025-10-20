#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

struct FTpGasGameplayTags
{
	
public:
	static const FTpGasGameplayTags& Get() { return GameplayTags; }
	static void InitializeNativeGameplayTags(); // C++ tan tags eklemek için kullandık

	// Attribute Tag
	FGameplayTag Attributes_Vital_Health;
	FGameplayTag Attributes_Vital_Mana;
	FGameplayTag Attributes_Vital_Stamina;
	
	FGameplayTag Attributes_Primary_Vigor;
	FGameplayTag Attributes_Primary_PhysicalDamage;
	FGameplayTag Attributes_Primary_MagicalDamage;
	FGameplayTag Attributes_Primary_DamageReduction;
	FGameplayTag Attributes_Primary_HealthRegeneration;
	FGameplayTag Attributes_Primary_ManaRegeneration;
	FGameplayTag Attributes_Primary_StaminaRegeneration;
	FGameplayTag Attributes_Primary_AttackSpeed;
	FGameplayTag Attributes_Primary_LifeSteal;
	FGameplayTag Attributes_Primary_CriticalHitChance;
	FGameplayTag Attributes_Primary_CriticalHitDamage;

	FGameplayTag Attributes_Secondary_MaxHealth;
	FGameplayTag Attributes_Secondary_MaxMana;
	FGameplayTag Attributes_Secondary_MaxStamina;

	// Ability Tag
	FGameplayTag Abilities_None;
	FGameplayTag Abilities_Test;
	FGameplayTag Abilities_Die;
	

	// Skill Menu Tags
	FGameplayTag Abilities_SkillStatus_Locked;
	FGameplayTag Abilities_SkillStatus_Eligible;
	FGameplayTag Abilities_SkillStatus_Unlocked;
	FGameplayTag Abilities_SkillStatus_Equipped;

	// Skill Type Tags
	FGameplayTag Abilities_Type_Offensive;
	FGameplayTag Abilities_Type_Passive;
	FGameplayTag Abilities_Type_None;

	// Skill Tag
	FGameplayTag Abilities_FireStaff_Skill1;
	FGameplayTag Abilities_FireStaff_Skill2;
	FGameplayTag Abilities_FireStaff_Skill3;
	FGameplayTag Abilities_FireStaff_Skill4;

	FGameplayTag Abilities_NatureStaff_Skill1;
	FGameplayTag Abilities_NatureStaff_Skill2;
	FGameplayTag Abilities_NatureStaff_Skill3;
	FGameplayTag Abilities_NatureStaff_Skill4;

	FGameplayTag Abilities_NecroStaff_Skill1;
	FGameplayTag Abilities_NecroStaff_Skill2;
	FGameplayTag Abilities_NecroStaff_Skill3;
	FGameplayTag Abilities_NecroStaff_Skill4;

	FGameplayTag Abilities_Bow_Skill1;
	FGameplayTag Abilities_Bow_Skill2;
	FGameplayTag Abilities_Bow_Skill3;
	FGameplayTag Abilities_Bow_Skill4;

	FGameplayTag Abilities_Spear_Skill1;
	FGameplayTag Abilities_Spear_Skill2;
	FGameplayTag Abilities_Spear_Skill3;
	FGameplayTag Abilities_Spear_Skill4;

	FGameplayTag Abilities_OneHandedSword_Skill1;
	FGameplayTag Abilities_OneHandedSword_Skill2;
	FGameplayTag Abilities_OneHandedSword_Skill3;
	FGameplayTag Abilities_OneHandedSword_Skill4;

	FGameplayTag Abilities_SwordAndShield_Skill1;
	FGameplayTag Abilities_SwordAndShield_Skill2;
	FGameplayTag Abilities_SwordAndShield_Skill3;
	FGameplayTag Abilities_SwordAndShield_Skill4;

	FGameplayTag Abilities_Boss_CloseDistanceSkill;
	FGameplayTag Abilities_Boss_MidDistanceSkill;
	FGameplayTag Abilities_Boss_LongDistanceSkill;
	FGameplayTag Abilities_Boss_Dodge;
	FGameplayTag Abilities_Boss_Entrance;
	FGameplayTag Abilities_Boss_StartSecondPhase;

	// Skill Cooldown Tag
	FGameplayTag Cooldown_FireStaff_Skill1;
	FGameplayTag Cooldown_FireStaff_Skill2;
	FGameplayTag Cooldown_FireStaff_Skill3;
	FGameplayTag Cooldown_FireStaff_Skill4;

	FGameplayTag Cooldown_NatureStaff_Skill1;
	FGameplayTag Cooldown_NatureStaff_Skill2;
	FGameplayTag Cooldown_NatureStaff_Skill3;
	FGameplayTag Cooldown_NatureStaff_Skill4;

	FGameplayTag Cooldown_NecroStaff_Skill1;
	FGameplayTag Cooldown_NecroStaff_Skill2;
	FGameplayTag Cooldown_NecroStaff_Skill3;
	FGameplayTag Cooldown_NecroStaff_Skill4;

	FGameplayTag Cooldown_Bow_Skill1;
	FGameplayTag Cooldown_Bow_Skill2;
	FGameplayTag Cooldown_Bow_Skill3;
	FGameplayTag Cooldown_Bow_Skill4;
	FGameplayTag Cooldown_Bow_RMB;

	FGameplayTag Cooldown_Spear_Skill1;
	FGameplayTag Cooldown_Spear_Skill2;
	FGameplayTag Cooldown_Spear_Skill3;
	FGameplayTag Cooldown_Spear_Skill4;

	FGameplayTag Cooldown_OneHandedSword_Skill1;
	FGameplayTag Cooldown_OneHandedSword_Skill2;
	FGameplayTag Cooldown_OneHandedSword_Skill3;
	FGameplayTag Cooldown_OneHandedSword_Skill4;

	FGameplayTag Cooldown_SwordAndShield_Skill1;
	FGameplayTag Cooldown_SwordAndShield_Skill2;
	FGameplayTag Cooldown_SwordAndShield_Skill3;
	FGameplayTag Cooldown_SwordAndShield_Skill4;

	
	
	// Passive Skill Tag
	FGameplayTag Abilities_Passive_HaloOfProtection;
	FGameplayTag Abilities_Passive_LifeSiphon;
	FGameplayTag Abilities_Passive_ManaSiphon;

	// Combat Ability Tag
	FGameplayTag Abilities_Status_BasicHitReact;
	FGameplayTag Abilities_Status_HeavyHitReact;
	FGameplayTag Abilities_Status_Stun;
	FGameplayTag Abilities_Status_BlockHitted;
	FGameplayTag Abilities_Status_BlockBroken;
	
	FGameplayTag Abilities_Combat_LightAttack;
	FGameplayTag Abilities_Combat_ToCloseReact;
	FGameplayTag Abilities_Combat_Dodge;
	FGameplayTag Abilities_Combat_Guarding;
	FGameplayTag Abilities_Combat_FlameThrowing;
	FGameplayTag Abilities_Combat_BowRMB;
	FGameplayTag Abilities_Combat_HeavyAttack;
	FGameplayTag Abilities_Combat_Skill_1;
	FGameplayTag Abilities_Combat_Skill_2;
	FGameplayTag Abilities_Combat_Skill_3;
	FGameplayTag Abilities_Combat_Skill_4;
	FGameplayTag Abilities_Combat_Sprint;

	// Ai Ability Tag
	FGameplayTag Abilities_Ai_CloseDistanceSkillAttack;
	FGameplayTag Abilities_Ai_LongDistanceSkillAttack;

	// Ai Effects Tag
	FGameplayTag Effects_Ai_AttackCooldown;
	FGameplayTag Effects_Ai_SummonIsFriend;
	

	// Effect Block Tag
	FGameplayTag Effects_Block_BasicHitReact;
	FGameplayTag Effects_Block_HeavyHitReact;
	FGameplayTag Effects_Block_Stun;
	FGameplayTag Effects_Block_AllDamage;

	// Equip - UnEquip Tag
	FGameplayTag Effects_WeaponEquipped_UnEquipped;
	FGameplayTag Effects_WeaponEquipped_SwordAndShield;
	FGameplayTag Effects_WeaponEquipped_OneHandedSword;
	FGameplayTag Effects_WeaponEquipped_Spear;
	FGameplayTag Effects_WeaponEquipped_TwoHandedAxe;
	FGameplayTag Effects_WeaponEquipped_Bow;
	FGameplayTag Effects_WeaponEquipped_Staff_FireStaff;
	FGameplayTag Effects_WeaponEquipped_Staff_NecroStaff;
	FGameplayTag Effects_WeaponEquipped_Staff_NatureStaff;

	// Debuff Tag
	FGameplayTag Effects_Debuff_Burning;

	// Buff Tag
	FGameplayTag Effects_Buff_NatureStaffSkill4;

	// Damage Type Tag
	FGameplayTag Damage;
	FGameplayTag Heal;

	// Input Tag
	FGameplayTag InputTag_LMB;
	FGameplayTag InputTag_ShiftLMB;
	FGameplayTag InputTag_RMB;
	FGameplayTag InputTag_Space;
	FGameplayTag InputTag_1;
	FGameplayTag InputTag_2;
	FGameplayTag InputTag_3;
	FGameplayTag InputTag_4;
	FGameplayTag InputTag_Passive_1;
	FGameplayTag InputTag_Passive_2;

	// Regeneration Effect Tag 
	FGameplayTag Effects_Regeneration_Health;
	FGameplayTag Effects_Regeneration_Mana;
	FGameplayTag Effects_Regeneration_Stamina;

	// Sprint Effect Tag
	FGameplayTag Effects_Sprinting;

	// Effect Attacking Effect Tag
	FGameplayTag Effects_Attacking;

	// After Dodge Attached Tag (For after dodge special attack)
	FGameplayTag Effects_Combat_CanDodgeForwardAttack;
	FGameplayTag Effects_Combat_CanDodgeBackwardAttack;
	
	// Pvp Disable Effect
	FGameplayTag Effects_PvpDisabled;

	// Cooldown Tag
	FGameplayTag Effects_Cooldown_Skill_1;
	FGameplayTag Effects_Cooldown_Skill_2;
	FGameplayTag Effects_Cooldown_Skill_3;
	FGameplayTag Effects_Cooldown_Skill_4;

	FGameplayTag Effects_Cooldown_Boss_SkillAttack;
	FGameplayTag Effects_Cooldown_Boss_Dodge;

	FGameplayTag Cooldown_Ai_ToCloseReact;

	// Meta Tag
	FGameplayTag Attributes_Meta_IncomingXP;
	

private:
	static FTpGasGameplayTags GameplayTags;

	

};

