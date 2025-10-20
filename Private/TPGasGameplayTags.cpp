// Fill out your copyright notice in the Description page of Project Settings.


#include "TPGasGameplayTags.h"
#include "GameplayTagsManager.h"

FTpGasGameplayTags FTpGasGameplayTags::GameplayTags;

void FTpGasGameplayTags::InitializeNativeGameplayTags()
{
	/*
	 * Test Ability Tag
	 */
	GameplayTags.Abilities_Test = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Test"), FString("Test"));

	GameplayTags.Abilities_None = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.None"), FString("None Tag"));
	
	GameplayTags.Abilities_Die = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Die"), FString("Die Tag"));

	
	/*
	 * Damage Type Tag
	 */
	GameplayTags.Damage= UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Damage"), FString("Damage Tag"));

	GameplayTags.Heal= UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Heal"), FString("Heal Tag"));
	

	
	/*
	 * Input Tags
	 */
	GameplayTags.InputTag_LMB = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.LMB"), FString("Input LMB Tag"));

	GameplayTags.InputTag_ShiftLMB = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.ShiftLMB"), FString("Shift + LMB input Tag"));

	GameplayTags.InputTag_RMB = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.RMB"), FString("Input RMB Tag"));

	GameplayTags.InputTag_Space = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.Space"), FString("Input Space Tag"));

	GameplayTags.InputTag_1 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.1"), FString("Input 1 Tag"));
	
	GameplayTags.InputTag_2 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.2"), FString("Input 2 Tag"));

	GameplayTags.InputTag_3 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.3"), FString("Input 3 Tag"));

	GameplayTags.InputTag_4 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.4"), FString("Input 4 Tag"));

	GameplayTags.InputTag_Passive_1 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.Passive.1"), FString("Passive 1 Tag"));

	GameplayTags.InputTag_Passive_2 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.Passive.2"), FString("Passive 2 Tag"));


	
	/*
	 * Combat React Ability Tags
	 */
	GameplayTags.Abilities_Status_BasicHitReact = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Status.BasicHitReact"), FString("BasicHitReact Ability Tag"));

	GameplayTags.Abilities_Status_HeavyHitReact = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Status.HeavyHitReact"), FString("HeavyHitReact Ability Tag"));

	GameplayTags.Abilities_Status_Stun = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Status.Stun"), FString("Stun Ability Tag"));

	GameplayTags.Abilities_Status_BlockHitted = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Status.BlockHitted"), FString("BlockHitted Ability Tag"));

	GameplayTags.Abilities_Status_BlockBroken = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Status.BlockBroken"), FString("BlockBroken Ability Tag"));

	
	/*
	 * Combat Ability Tags
	 */
	GameplayTags.Abilities_Combat_Dodge = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Combat.Dodge"), FString("Dodge Ability Tag"));

	GameplayTags.Abilities_Combat_Sprint = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Combat.Sprint"), FString("Sprint Ability Tag"));

	GameplayTags.Abilities_Combat_Guarding = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Combat.Guarding"), FString("Guarding Ability Tag"));
	
	GameplayTags.Abilities_Combat_LightAttack = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Combat.LightAttack"), FString("Light Attack Ability Tag"));

	GameplayTags.Abilities_Combat_ToCloseReact = UGameplayTagsManager::Get().AddNativeGameplayTag(
    		FName("Abilities.Combat.ToCloseReact"), FString("For Ranged Enemies"));

	GameplayTags.Abilities_Combat_HeavyAttack = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Combat.HeavyAttack"), FString("Heavy Attack Ability Tag"));

	GameplayTags.Abilities_Combat_Skill_1 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Combat.Skill.1"), FString("Weapon Skill 1 Ability Tag"));

	GameplayTags.Abilities_Combat_Skill_2 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Combat.Skill.2"), FString("Weapon Skill 2 Ability Tag"));

	GameplayTags.Abilities_Combat_Skill_3 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Combat.Skill.3"), FString("Weapon Skill 3 Ability Tag"));

	GameplayTags.Abilities_Combat_Skill_4 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Combat.Skill.4"), FString("Weapon Skill 4 Ability Tag"));

	GameplayTags.Abilities_Combat_FlameThrowing = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Combat.FlameThrowing"), FString("Flame Throwing Ability Tag"));

	GameplayTags.Abilities_Combat_BowRMB = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Combat.BowRMB"), FString("Bow RMB Ability Tag"));


	/*
	 * Ai Ability Tags
	 */
	GameplayTags.Abilities_Ai_CloseDistanceSkillAttack = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Ai.CloseDistanceSkillAttack"), FString("CloseDistanceSkillAttack Ability Tag"));

	GameplayTags.Abilities_Ai_LongDistanceSkillAttack = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Ai.LongDistanceSkillAttack"), FString("LongDistanceSkillAttack Ability Tag"));

	GameplayTags.Effects_Ai_AttackCooldown = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Effects.Ai.AttackCooldown"), FString("Ai Action Cooldown Ability Tag"));
	
	GameplayTags.Effects_Ai_SummonIsFriend = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Effects.Ai.SummonIsFriend"), FString("Summoned Ai is enemy for other players"));


	/*
	 * Effect Block Tags
	 */
	GameplayTags.Effects_Block_BasicHitReact = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Effects.Block.BasicHitReact"), FString("This Tag Block Basic Hit React Ability"));

	GameplayTags.Effects_Block_HeavyHitReact = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Effects.Block.HeavyHitReact"), FString("This Tag Block Heavy Hit React Ability"));

	GameplayTags.Effects_Block_Stun = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Effects.Block.Stun"), FString("This Tag Block Stun React Ability"));

	GameplayTags.Effects_Block_AllDamage = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Effects.Block.AllDamage"), FString("This Tag Block Stun React Ability"));
	

	/*
	 * Equip - UnEquip Weapon Tags
	 */
	GameplayTags.Effects_WeaponEquipped_UnEquipped = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Effects.WeaponEquipped.UnEquipped"), FString("Weapon UnEquipped Tag Attach"));

	GameplayTags.Effects_WeaponEquipped_Spear = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Effects.WeaponEquipped.SwordAndShield"), FString("Weapon SwordAndShield Equipped Tag Attach"));

	GameplayTags.Effects_WeaponEquipped_OneHandedSword = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Effects.WeaponEquipped.OneHandedSword"), FString("Weapon OneHandedSword Equipped Tag Attach"));

	GameplayTags.Effects_WeaponEquipped_Spear = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Effects.WeaponEquipped.Spear"), FString("Weapon Spear Equipped Tag Attach"));

	GameplayTags.Effects_WeaponEquipped_Bow = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Effects.WeaponEquipped.Bow"), FString("Weapon Bow Equipped Tag Attach"));

	GameplayTags.Effects_WeaponEquipped_TwoHandedAxe = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Effects.WeaponEquipped.TwoHandedAxe"), FString("Weapon TwoHandedAxe Equipped Tag Attach"));

	GameplayTags.Effects_WeaponEquipped_Staff_FireStaff = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Effects.WeaponEquipped.Staff.FireStaff"), FString("Weapon FireStaff Equipped Tag Attach"));

	GameplayTags.Effects_WeaponEquipped_Staff_NecroStaff = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Effects.WeaponEquipped.Staff.NecroStaff"), FString("Weapon NecroStaff Equipped Tag Attach"));

	GameplayTags.Effects_WeaponEquipped_Staff_NatureStaff = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Effects.WeaponEquipped.Staff.NatureStaff"), FString("Weapon NatureStaff Equipped Tag Attach"));
	

	/*
	 * Debuff Tag
	 */

	GameplayTags.Effects_Debuff_Burning = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Effects.Debuff.Burning"), FString("Burning Tag Attach"));


	
	/*
	 * Buff Tag
	 */
	GameplayTags.Effects_Buff_NatureStaffSkill4 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Effects.Buff.NatureStaffSkill4"), FString("NatureStaffSkill4 Buff Applied Tag"));
	
	
	/*
	 * Attributes Tag
	 */
	GameplayTags.Attributes_Vital_Health = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Vital.Health"), FString("Health Attribute Tag"));

	GameplayTags.Attributes_Vital_Mana = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Vital.Mana"), FString("Mana Attribute Tag"));

	GameplayTags.Attributes_Vital_Stamina = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Vital.Stamina"), FString("Stamina Attribute Tag"));
	
	
	
	GameplayTags.Attributes_Primary_Vigor = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Primary.Vigor"), FString("Vigor Attribute Tag"));

	GameplayTags.Attributes_Primary_PhysicalDamage = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Primary.PhysicalDamage"), FString("PhysicalDamage Attribute Tag"));
		
	GameplayTags.Attributes_Primary_MagicalDamage = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Primary.MagicalDamage"), FString("MagicalDamage Attribute Tag"));

	GameplayTags.Attributes_Primary_DamageReduction = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Primary.DamageReduction"), FString("DamageReduction Attribute Tag"));

	GameplayTags.Attributes_Primary_HealthRegeneration = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Primary.HealthRegeneration"), FString("DamageReduction Attribute Tag"));

	GameplayTags.Attributes_Primary_ManaRegeneration = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Primary.ManaRegeneration"), FString("ManaRegeneration Attribute Tag"));

	GameplayTags.Attributes_Primary_StaminaRegeneration = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Primary.StaminaRegeneration"), FString("StaminaRegeneration Attribute Tag"));

	GameplayTags.Attributes_Primary_AttackSpeed = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Primary.AttackSpeed"), FString("AttackSpeed Attribute Tag"));
	
	GameplayTags.Attributes_Primary_LifeSteal = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Primary.LifeSteal"), FString("LifeSteal Attribute Tag"));

	GameplayTags.Attributes_Primary_CriticalHitChance = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Primary.CriticalHitChance"), FString("CriticalHitChance Attribute Tag"));

	GameplayTags.Attributes_Primary_CriticalHitDamage = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Primary.CriticalHitDamage"), FString("CriticalHitDamage Attribute Tag"));

	

	GameplayTags.Attributes_Secondary_MaxHealth = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.MaxHealth"), FString("Max Health Attribute Tag"));

	GameplayTags.Attributes_Secondary_MaxMana = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.MaxMana"), FString("Max Mana Attribute Tag"));

	GameplayTags.Attributes_Secondary_MaxStamina = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.MaxStamina"), FString("Max Stamina Attribute Tag"));
	


	/*
	 * Regeneration Effect Tag
	 */

	GameplayTags.Effects_Regeneration_Health = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Effects.Regeneration.Health"), FString("Attach this tag when heal regen is active"));

	GameplayTags.Effects_Regeneration_Mana = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Effects.Regeneration.Mana"), FString("Attach this tag when Mana regen is active"));

	GameplayTags.Effects_Regeneration_Stamina = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Effects.Regeneration.Stamina"), FString("Attach this tag when Stamina regen is active"));


	/*
	 * Sprint Effect Tag
	 */

	GameplayTags.Effects_Sprinting = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Effects.Sprinting"), FString("Sprinting tag attach"));


	/*
	 * Attacking Effect Tag
	 */

	GameplayTags.Effects_Attacking = UGameplayTagsManager::Get().AddNativeGameplayTag(
    		FName("Effects.Attacking"), FString("Attacking tag attach"));


	/*
	 * Can After Dodge Special Attack Tag
	 */
	GameplayTags.Effects_Combat_CanDodgeForwardAttack = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Effects.Combat.CanDodgeForwardAttack"), FString("After Dodge forward attack tag attach"));

	GameplayTags.Effects_Combat_CanDodgeBackwardAttack = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Effects.Combat.CanDodgeBackwardAttack"), FString("After Dodge backward attack tag attach"));
	

	/*
	 * PVP Disabled Effect 
	 */

	GameplayTags.Effects_PvpDisabled = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Effects.PvpDisabled"), FString("When this tag active, player cant damage other player."));


	/*
	 * Cooldown Tags
	 */

	GameplayTags.Effects_Cooldown_Skill_1 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Effects.Cooldown.Skill.1"), FString("Skill 1 Cooldown Tag"));

	GameplayTags.Effects_Cooldown_Skill_2 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Effects.Cooldown.Skill.2"), FString("Skill 2 Cooldown Tag"));

	GameplayTags.Effects_Cooldown_Skill_3 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Effects.Cooldown.Skill.3"), FString("Skill 3 Cooldown Tag"));

	GameplayTags.Effects_Cooldown_Skill_4 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Effects.Cooldown.Skill.4"), FString("Skill 4 Cooldown Tag"));


	GameplayTags.Effects_Cooldown_Boss_SkillAttack = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Effects.Cooldown.Boss.SkillAttack"), FString("Boss Skill Attack Cooldown"));

	GameplayTags.Effects_Cooldown_Boss_Dodge = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Effects.Cooldown.Boss.Dodge"), FString("Boss Dodge Cooldown"));


	/*
	 * Skill Menu Skill Status Tags
	 */

	GameplayTags.Abilities_SkillStatus_Locked = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.SkillStatus.Locked"), FString("Skill is locked"));

	GameplayTags.Abilities_SkillStatus_Eligible = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.SkillStatus.Eligible"), FString("Skill 1s eligible"));

	GameplayTags.Abilities_SkillStatus_Unlocked = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.SkillStatus.Unlocked"), FString("Skill is unlocked"));

	GameplayTags.Abilities_SkillStatus_Equipped = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.SkillStatus.Equipped"), FString("Skill is equipped"));


	/*
	 * Skill Menu Skill Status Tags
	 */

	GameplayTags.Abilities_Type_Offensive = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Type.Offensive"), FString("Offensive Skill"));

	GameplayTags.Abilities_Type_Passive = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Type.Passive"), FString("Passive Skill"));

	GameplayTags.Abilities_Type_None= UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Type.None"), FString("Offensive Skill"));


	/*
	 * Skill Tags
	 */

	GameplayTags.Abilities_FireStaff_Skill1 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.FireStaff.Skill1"), FString("FireStaff Skill1 Tag"));

	GameplayTags.Abilities_FireStaff_Skill2 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.FireStaff.Skill2"), FString("FireStaff Skill2 Tag"));

	GameplayTags.Abilities_FireStaff_Skill3 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.FireStaff.Skill3"), FString("FireStaff Skill3 Tag"));

	GameplayTags.Abilities_FireStaff_Skill4 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.FireStaff.Skill4"), FString("FireStaff Skill4 Tag"));

	
	GameplayTags.Abilities_NatureStaff_Skill1 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.NatureStaff.Skill1"), FString("NatureStaff Skill1 Tag"));

	GameplayTags.Abilities_NatureStaff_Skill2 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.NatureStaff.Skill2"), FString("NatureStaff Skill2 Tag"));

	GameplayTags.Abilities_NatureStaff_Skill3 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.NatureStaff.Skill3"), FString("NatureStaff Skill3 Tag"));

	GameplayTags.Abilities_NatureStaff_Skill4 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.NatureStaff.Skill4"), FString("NatureStaff Skill4 Tag"));

	
	GameplayTags.Abilities_NecroStaff_Skill1 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.NecroStaff.Skill1"), FString("NecroStaff Skill1 Tag"));

	GameplayTags.Abilities_NecroStaff_Skill2 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.NecroStaff.Skill2"), FString("NecroStaff Skill2 Tag"));

	GameplayTags.Abilities_NecroStaff_Skill3 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.NecroStaff.Skill3"), FString("NecroStaff Skill3 Tag"));
	
	GameplayTags.Abilities_NecroStaff_Skill4 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.NecroStaff.Skill4"), FString("NecroStaff Skill4 Tag"));


	GameplayTags.Abilities_Bow_Skill1 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Bow.Skill1"), FString("Bow Skill1 Tag"));

	GameplayTags.Abilities_Bow_Skill2 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Bow.Skill2"), FString("Bow Skill2 Tag"));

	GameplayTags.Abilities_Bow_Skill3 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Bow.Skill3"), FString("Bow Skill3 Tag"));

	GameplayTags.Abilities_Bow_Skill4 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Bow.Skill4"), FString("Bow Skill4 Tag"));


	GameplayTags.Abilities_Spear_Skill1 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Spear.Skill1"), FString("Spear Skill1 Tag"));

	GameplayTags.Abilities_Spear_Skill2 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Spear.Skill2"), FString("Spear Skill2 Tag"));

	GameplayTags.Abilities_Spear_Skill3 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Spear.Skill3"), FString("Spear Skill3 Tag"));

	GameplayTags.Abilities_Spear_Skill4 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Spear.Skill4"), FString("Spear Skill4 Tag"));


	GameplayTags.Abilities_OneHandedSword_Skill1 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.OneHandedSword.Skill1"), FString("OneHandedSword Skill1 Tag"));

	GameplayTags.Abilities_OneHandedSword_Skill2 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.OneHandedSword.Skill2"), FString("OneHandedSword Skill2 Tag"));

	GameplayTags.Abilities_OneHandedSword_Skill3 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.OneHandedSword.Skill3"), FString("OneHandedSword Skill3 Tag"));

	GameplayTags.Abilities_OneHandedSword_Skill4 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.OneHandedSword.Skill4"), FString("OneHandedSword Skill4 Tag"));


	GameplayTags.Abilities_SwordAndShield_Skill1 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.SwordAndShield.Skill1"), FString("SwordAndShield Skill1 Tag"));

	GameplayTags.Abilities_SwordAndShield_Skill2 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.SwordAndShield.Skill2"), FString("SwordAndShield Skill2 Tag"));

	GameplayTags.Abilities_SwordAndShield_Skill3 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.SwordAndShield.Skill3"), FString("SwordAndShield Skill3 Tag"));

	GameplayTags.Abilities_SwordAndShield_Skill4 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.SwordAndShield.Skill4"), FString("SwordAndShield Skill4 Tag"));


	GameplayTags.Abilities_Boss_CloseDistanceSkill = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Boss_CloseDistanceSkill"), FString("Boss Close Distance Skills abilities"));
	
	GameplayTags.Abilities_Boss_MidDistanceSkill = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Boss_MidDistanceSkill"), FString("Boss Mid Distance Skills abilities"));

	GameplayTags.Abilities_Boss_LongDistanceSkill = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Boss_LongDistanceSkill"), FString("Boss Long Distance Skills abilities"));
	
	GameplayTags.Abilities_Boss_Dodge = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Boss_Dodge"), FString("Boss Dodge abilitity tag"));

	GameplayTags.Abilities_Boss_Entrance = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Boss.Entrance"), FString("Boss Fight Start Entrance Ability Tag"));

	GameplayTags.Abilities_Boss_StartSecondPhase = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Boss.StartSecondPhase"), FString("Boss Start Second Phase Ability Tag"));




	/*
	 * Skill Cooldown Tags
	 */

	GameplayTags.Cooldown_FireStaff_Skill1 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Cooldown.FireStaff.Skill1"), FString("FireStaff Skill1 Cooldown Tag"));

	GameplayTags.Cooldown_FireStaff_Skill2 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Cooldown.FireStaff.Skill2"), FString("FireStaff Skill2 Cooldown Tag"));

	GameplayTags.Cooldown_FireStaff_Skill3 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Cooldown.FireStaff.Skill3"), FString("FireStaff Skill3 Cooldown Tag"));

	GameplayTags.Cooldown_FireStaff_Skill4 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Cooldown.FireStaff.Skill4"), FString("FireStaff Skill4 Cooldown Tag"));


	GameplayTags.Cooldown_NatureStaff_Skill1 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Cooldown.NatureStaff.Skill1"), FString("NatureStaff Skill1 Cooldown Tag"));

	GameplayTags.Cooldown_NatureStaff_Skill2 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Cooldown.NatureStaff.Skill2"), FString("NatureStaff Skill2 Cooldown Tag"));

	GameplayTags.Cooldown_NatureStaff_Skill3 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Cooldown.NatureStaff.Skill3"), FString("NatureStaff Skill3 Cooldown Tag"));

	GameplayTags.Cooldown_NatureStaff_Skill4 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Cooldown.NatureStaff.Skill4"), FString("NatureStaff Skill4 Cooldown Tag"));


	GameplayTags.Cooldown_NecroStaff_Skill1 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Cooldown.NecroStaff.Skill1"), FString("NecroStaff Skill1 Cooldown Tag"));

	GameplayTags.Cooldown_NecroStaff_Skill2 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Cooldown.NecroStaff.Skill2"), FString("NecroStaff Skill2 Cooldown Tag"));

	GameplayTags.Cooldown_NecroStaff_Skill3 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Cooldown.NecroStaff.Skill3"), FString("NecroStaff Skill3 Cooldown Tag"));

	GameplayTags.Cooldown_NecroStaff_Skill4 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Cooldown.NecroStaff.Skill4"), FString("NecroStaff Skill4 Cooldown Tag"));


	GameplayTags.Cooldown_Bow_Skill1 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Cooldown.Bow.Skill1"), FString("Bow Skill1 Cooldown Tag"));

	GameplayTags.Cooldown_Bow_Skill2 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Cooldown.Bow.Skill2"), FString("Bow Skill2 Cooldown Tag"));

	GameplayTags.Cooldown_Bow_Skill3 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Cooldown.Bow.Skill3"), FString("Bow Skill3 Cooldown Tag"));

	GameplayTags.Cooldown_Bow_Skill4 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Cooldown.Bow.Skill4"), FString("Bow Skill4 Cooldown Tag"));

	GameplayTags.Cooldown_Bow_RMB = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Cooldown.Bow.RMB"), FString("Bow RMB Cooldown Tag"));


	GameplayTags.Cooldown_Spear_Skill1 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Cooldown.Spear.Skill1"), FString("Spear Skill1 Cooldown Tag"));
	
	GameplayTags.Cooldown_Spear_Skill2 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Cooldown.Spear.Skill2"), FString("Spear Skill2 Cooldown Tag"));

	GameplayTags.Cooldown_Spear_Skill3 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Cooldown.Spear.Skill3"), FString("Spear Skill3 Cooldown Tag"));

	GameplayTags.Cooldown_Spear_Skill4 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Cooldown.Spear.Skill4"), FString("Spear Skill4 Cooldown Tag"));


	GameplayTags.Cooldown_OneHandedSword_Skill1 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Cooldown.OneHandedSword.Skill1"), FString("OneHandedSword Skill1 Cooldown Tag"));

	GameplayTags.Cooldown_OneHandedSword_Skill2 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Cooldown.OneHandedSword.Skill2"), FString("OneHandedSword Skill2 Cooldown Tag"));

	GameplayTags.Cooldown_OneHandedSword_Skill3 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Cooldown.OneHandedSword.Skill3"), FString("OneHandedSword Skill3 Cooldown Tag"));
	
	GameplayTags.Cooldown_OneHandedSword_Skill4 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Cooldown.OneHandedSword.Skill4"), FString("OneHandedSword Skill4 Cooldown Tag"));


	GameplayTags.Cooldown_SwordAndShield_Skill1 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Cooldown.SwordAndShield.Skill1"), FString("SwordAndShield Skill1 Cooldown Tag"));

	GameplayTags.Cooldown_SwordAndShield_Skill2 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Cooldown.SwordAndShield.Skill2"), FString("SwordAndShield Skill2 Cooldown Tag"));

	GameplayTags.Cooldown_SwordAndShield_Skill3 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Cooldown.SwordAndShield.Skill3"), FString("SwordAndShield Skill3 Cooldown Tag"));

	GameplayTags.Cooldown_SwordAndShield_Skill4 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Cooldown.SwordAndShield.Skill4"), FString("SwordAndShield Skill4 Cooldown Tag"));


	GameplayTags.Cooldown_Ai_ToCloseReact = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Cooldown.Ai.ToCloseReact"), FString("Ability cooldown for distant enemies that will run if we get too close"));


	/*
	 * Passive Skill Tags
	 */

	GameplayTags.Abilities_Passive_HaloOfProtection = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Passive.HaloOfProtection"), FString("HaloOfProtection Passive Skill Tag"));

	GameplayTags.Abilities_Passive_LifeSiphon = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Passive.LifeSiphon"), FString("LifeSiphon Passive Skill Tag"));

	GameplayTags.Abilities_Passive_ManaSiphon = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Passive.ManaSiphon"), FString("ManaSiphon Passive Skill Tag"));


	/*
	 * Meta Tags
	 */

	GameplayTags.Attributes_Meta_IncomingXP = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Meta.IncomingXP"), FString("Incoming XP Tag"));
	
}
