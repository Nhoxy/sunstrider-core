
#include <string>
#include "MapManager.h"
#include "ObjectMgr.h"
#include "Pet.h"

enum PetTypes {
    WINDSERPENT,
    SCORPID,
    SPIDER,
    RAVAGER,
    BOAR,
    RAPTOR,
    DRAGONHAWK
};

std::string getPetTypeName(PetTypes type) 
{
    switch(type)
    {
    case WINDSERPENT:   return "Wind serpent";
    case SCORPID:       return "Scorpid";
    case SPIDER:        return "Spider";
    case RAVAGER:       return "Ravager";
    case BOAR:          return "Boar";
    case RAPTOR:        return "Raptor";
    case DRAGONHAWK:    return "Dragonhawk";
    default:            return "";
    }
}

uint32 getPetTypeEntry(uint32 type) 
{
    switch(type)
    {
    case WINDSERPENT:    return 20749;
    case SCORPID:        return 21864;
    case SPIDER:         return 930;
    case RAVAGER:        return 16933;
    case BOAR:           return 1126;
    case RAPTOR:         return 20634;
    case DRAGONHAWK:     return 20038;
    default:             return 0;
    }
}

bool GossipHello_arenabeastmaster(Player *player, Creature *me)
{    
    if(player->GetClass() != CLASS_HUNTER)
    {
        me->Whisper("My services are reserved to hunters!", LANG_UNIVERSAL, player);
        return true;
    }

    player->ADD_GOSSIP_ITEM( GOSSIP_ICON_CHAT, getPetTypeName(WINDSERPENT), GOSSIP_SENDER_MAIN, WINDSERPENT);
    player->ADD_GOSSIP_ITEM( GOSSIP_ICON_CHAT, getPetTypeName(SCORPID),     GOSSIP_SENDER_MAIN, SCORPID);
    player->ADD_GOSSIP_ITEM( GOSSIP_ICON_CHAT, getPetTypeName(SPIDER),      GOSSIP_SENDER_MAIN, SPIDER);
    player->ADD_GOSSIP_ITEM( GOSSIP_ICON_CHAT, getPetTypeName(RAVAGER),     GOSSIP_SENDER_MAIN, RAVAGER);
    player->ADD_GOSSIP_ITEM( GOSSIP_ICON_CHAT, getPetTypeName(BOAR),        GOSSIP_SENDER_MAIN, BOAR);
    player->ADD_GOSSIP_ITEM( GOSSIP_ICON_CHAT, getPetTypeName(RAPTOR),      GOSSIP_SENDER_MAIN, RAPTOR);
    player->ADD_GOSSIP_ITEM( GOSSIP_ICON_CHAT, getPetTypeName(DRAGONHAWK),  GOSSIP_SENDER_MAIN, DRAGONHAWK);
        
    player->SEND_GOSSIP_MENU_TEXTID(1,me->GetGUID());

    return true;
}

//code from ChatHandler::HandleCreatePetCommand(const char* args)
bool GossipSelect_arenabeastmaster( Player* player, Creature* me, uint32 /* sender */, uint32 type)
{
    
    if(player->GetPet() || player->GetTemporaryUnsummonedPetNumber())
    {
        me->Whisper("Abandon your current pet first.", LANG_UNIVERSAL, player);
        player->PlayerTalkClass->SendCloseGossip();
        return true;
    }

    auto  pet = new Pet(HUNTER_PET);
    if(!pet)
        return false;

    if(!pet->CreateBaseAtCreatureEntry(getPetTypeEntry(type), me))
    {
        delete pet;
        return false;
    }

    pet->SetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE, player->GetFaction());
    pet->SetUInt64Value(UNIT_FIELD_SUMMONEDBY, player->GetGUID());
    pet->SetUInt64Value(UNIT_FIELD_CREATEDBY, player->GetGUID());

    if(!pet->InitStatsForLevel(player->GetLevel()))
    {
        delete pet;
        return false;
    }

    pet->SetUInt32Value(UNIT_FIELD_LEVEL,player->GetLevel()-1);
    pet->GetCharmInfo()->SetPetNumber(sObjectMgr->GeneratePetNumber(), true);
    pet->AIM_Initialize();

    pet->InitPetCreateSpells();
    pet->SetHealth(pet->GetMaxHealth());

    sMapMgr->CreateMap(pet->GetMapId(), pet)->Add(pet->ToCreature());

    // visual effect for levelup
    pet->SetUInt32Value(UNIT_FIELD_LEVEL,player->GetLevel());

    player->SetPet(pet);
    pet->SavePetToDB(PET_SAVE_AS_CURRENT);
    player->PetSpellInitialize();
     
    player->PlayerTalkClass->SendCloseGossip();
        
    pet->SetLoyaltyLevel(BEST_FRIEND);
    pet->SetPower(POWER_HAPPINESS,1050000); //maxed
    pet->SetTP(player->GetLevel()*(pet->GetLoyaltyLevel()-1) - pet->GetDispTP()); //350 when best friend at lvl 70

    return true;
}

void AddSC_arenabeastmaster()
{
    OLDScript *newscript;

    newscript = new OLDScript;
    newscript->Name="npc_arenabeastmaster";
    newscript->OnGossipHello = &GossipHello_arenabeastmaster;
    newscript->OnGossipSelect = &GossipSelect_arenabeastmaster;
    sScriptMgr->RegisterOLDScript(newscript);
}