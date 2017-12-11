/* Copyright (C) 2006 - 2009 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

/* ScriptData
SDName: Wailing Caverns
SD%Complete: 95
SDComment: Need to add skill usage for Disciple of Naralex
SDCategory: Wailing Caverns
EndScriptData */

/* ContentData
EndContentData */


#include "EscortAI.h"
#include "def_wailing_caverns.h"

/*######
## npc_disciple_of_naralex
######*/

enum eEnums
{
    //say
    SAY_MAKE_PREPARATIONS         = -1043001,
    SAY_TEMPLE_OF_PROMISE         = -1043002,
    SAY_MUST_CONTINUE             = -1043003,
    SAY_BANISH_THE_SPIRITS        = -1043004,
    SAY_CAVERNS_PURIFIED          = -1043005,
    SAY_BEYOND_THIS_CORRIDOR      = -1043006,
    SAY_EMERALD_DREAM             = -1043007,
    SAY_MUTANUS_THE_DEVOURER      = -1043012,
    SAY_NARALEX_AWAKES            = -1043014,
    SAY_THANK_YOU                 = -1043015,
    SAY_FAREWELL                  = -1043016,
    SAY_ATTACKED                  = -1043017,
    //yell
    SAY_AT_LAST                   = -1043000,
    SAY_I_AM_AWAKE                = -1043013,
    //emote
    EMOTE_AWAKENING_RITUAL        = -1043008,
    EMOTE_TROUBLED_SLEEP          = -1043009,
    EMOTE_WRITHE_IN_AGONY         = -1043010,
    EMOTE_HORRENDOUS_VISION       = -1043011,
    //spell
    SPELL_MARK_OF_THE_WILD_RANK_2 = 5232,
    SPELL_SERPENTINE_CLEANSING    = 6270,
    SPELL_NARALEXS_AWAKENING      = 6271,
    SPELL_FLIGHT_FORM             = 33943,
    //npc entry
    NPC_DEVIATE_RAVAGER           = 3636,
    NPC_DEVIATE_VIPER             = 5755,
    NPC_DEVIATE_MOCCASIN          = 5762,
    NPC_NIGHTMARE_ECTOPLASM       = 5763,
    NPC_MUTANUS_THE_DEVOURER      = 3654,
};

#define GOSSIP_ID_START_1       698  //Naralex sleeps again!
#define GOSSIP_ID_START_2       699  //The fanglords are dead!
#define GOSSIP_ITEM_NARALEX     "Allons-y !"


class npc_disciple_of_naralex : public CreatureScript
{
public:
    npc_disciple_of_naralex() : CreatureScript("npc_disciple_of_naralex")
    { }

    class npc_disciple_of_naralexAI : public EscortAI
    {
        public:
        npc_disciple_of_naralexAI(Creature *c) : EscortAI(c)
        {
            pInstance = ((InstanceScript*)c->GetInstanceScript());
            eventTimer = 0;
            currentEvent = 0;
            eventProgress = 0;
            me->SetKeepActive(30 * MINUTE*IN_MILLISECONDS);
            me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC);
            me->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);   // I don't know if flags are reloaded from template in case NPC is respawned after death
        }
    
        uint32 eventTimer;
        uint32 currentEvent;
        uint32 eventProgress;
        InstanceScript* pInstance;
    
        void WaypointReached(uint32 i, uint32 pathID)
        override {
            if (!pInstance)
                return;
    
            switch (i)
            {
                case 4:
                    eventProgress = 1;
                    currentEvent = TYPE_NARALEX_PART1;
                    pInstance->SetData(TYPE_NARALEX_PART1, IN_PROGRESS);
                break;
                case 5:
                    DoScriptText(SAY_MUST_CONTINUE, me);
                    pInstance->SetData(TYPE_NARALEX_PART1, DONE);
                break;
                case 11:
                    eventProgress = 1;
                    currentEvent = TYPE_NARALEX_PART2;
                    pInstance->SetData(TYPE_NARALEX_PART2, IN_PROGRESS);
                break;
                case 12:
                    ++eventProgress;
                    me->SetHomePosition(me->GetPosition());
                    break;
                case 19:
                    DoScriptText(SAY_BEYOND_THIS_CORRIDOR, me);
                break;
                case 24:
                    eventProgress = 1;
                    currentEvent = TYPE_NARALEX_PART3;
                    pInstance->SetData(TYPE_NARALEX_PART3, IN_PROGRESS);
                break;
            }
        }
    
        void Reset()
        override {
    
        }
    
        void EnterCombat(Unit* who)
        override {
            DoScriptText(SAY_ATTACKED, me, who);
        }
    
        void JustDied(Unit *slayer)
        override {
            if (pInstance)
            {
                pInstance->SetData(TYPE_NARALEX_EVENT, FAIL);
                pInstance->SetData(TYPE_NARALEX_PART1, FAIL);
                pInstance->SetData(TYPE_NARALEX_PART2, FAIL);
                pInstance->SetData(TYPE_NARALEX_PART3, FAIL);
            }
        }
    
        void JustSummoned(Creature* summoned)
        override {
             summoned->AI()->AttackStart(me);
        }
        
        void EnterEvadeMode(EvadeReason why) override {
            if (currentEvent == TYPE_NARALEX_PART3 || (currentEvent == TYPE_NARALEX_PART2 && eventProgress == 2) || me->HasUnitState(UNIT_STATE_CASTING))
                return;
            else
                EscortAI::EnterEvadeMode(why);
        }
    
        void UpdateAI(const uint32 diff)
        override {
            if (currentEvent != TYPE_NARALEX_PART3)
                EscortAI::UpdateAI(diff);
    
            if (!pInstance)
                return;
            if (eventTimer <= diff)
            {
                eventTimer = 0;
                if (pInstance->GetData(currentEvent) == IN_PROGRESS)
                {
                    switch (currentEvent)
                    {
                        case TYPE_NARALEX_PART1:
                            if (eventProgress == 1)
                            {
                                ++eventProgress;
                                DoScriptText(SAY_TEMPLE_OF_PROMISE, me);
                                me->SummonCreature(NPC_DEVIATE_RAVAGER, -82.1763, 227.874, -93.3233, 0, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 5000);
                                me->SummonCreature(NPC_DEVIATE_RAVAGER, -72.9506, 216.645, -93.6756, 0, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 5000);
                            }
                        break;
                        case TYPE_NARALEX_PART2:
                            if (eventProgress == 1)
                            {
                                ++eventProgress;
                                DoScriptText(SAY_BANISH_THE_SPIRITS, me);
                                DoCast(me, SPELL_SERPENTINE_CLEANSING);
                                me->AddUnitState(UNIT_STATE_ROOT);
                                CAST_AI(EscortAI, me->AI())->SetCanAttack(false);
                                eventTimer = 30000;
                                me->SummonCreature(NPC_DEVIATE_VIPER, -61.5261, 273.676, -92.8442, 0, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 5000);
                                me->SummonCreature(NPC_DEVIATE_VIPER, -58.4658, 280.799, -92.8393, 0, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 5000);
                                me->SummonCreature(NPC_DEVIATE_VIPER, -50.002,  278.578, -92.8442, 0, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 5000);
                            }
                            else
                            if (eventProgress == 2)
                            {
                                CAST_AI(EscortAI, me->AI())->SetCanAttack(true);
                                me->ClearUnitState(UNIT_STATE_ROOT);
                                DoScriptText(SAY_CAVERNS_PURIFIED, me);
                                pInstance->SetData(TYPE_NARALEX_PART2, DONE);
                                if (me->HasAuraEffect(SPELL_SERPENTINE_CLEANSING))
                                    me->RemoveAura(SPELL_SERPENTINE_CLEANSING, 0);
                            }
                        break;
                        case TYPE_NARALEX_PART3:
                            if (eventProgress == 1)
                            {
                                ++eventProgress;
                                eventTimer = 4000;
                                me->SetStandState(UNIT_STAND_STATE_KNEEL);
                                DoScriptText(SAY_EMERALD_DREAM, me);
                            }
                            else
                            if (eventProgress == 2)
                            {
                                ++eventProgress;
                                eventTimer = 15000;
                                CAST_AI(EscortAI, me->AI())->SetCanAttack(false);
                                if (Creature* naralex = pInstance->instance->GetCreature(pInstance->GetData64(DATA_NARALEX)))
                                    DoCast(naralex, SPELL_NARALEXS_AWAKENING, true);
                                DoScriptText(EMOTE_AWAKENING_RITUAL, me);
                            }
                            else
                            if (eventProgress == 3)
                            {
                                ++eventProgress;
                                eventTimer = 15000;
                                if (Creature* naralex = pInstance->instance->GetCreature(pInstance->GetData64(DATA_NARALEX)))
                                    DoScriptText(EMOTE_TROUBLED_SLEEP, naralex);
                                me->SummonCreature(NPC_DEVIATE_MOCCASIN, 135.943, 199.701, -103.529, 0, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 15000);
                                me->SummonCreature(NPC_DEVIATE_MOCCASIN, 151.08,  221.13,  -103.609, 0, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 15000);
                                me->SummonCreature(NPC_DEVIATE_MOCCASIN, 128.007, 227.428, -97.421, 0, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 15000);
                            }
                            else
                            if (eventProgress == 4)
                            {
                                ++eventProgress;
                                eventTimer = 30000;
                                if (Creature* naralex = pInstance->instance->GetCreature(pInstance->GetData64(DATA_NARALEX)))
                                    DoScriptText(EMOTE_WRITHE_IN_AGONY, naralex);
                                me->SummonCreature(NPC_NIGHTMARE_ECTOPLASM, 133.413, 207.188, -102.469, 0, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 15000);
                                me->SummonCreature(NPC_NIGHTMARE_ECTOPLASM, 142.857, 218.645, -102.905, 0, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 15000);
                                me->SummonCreature(NPC_NIGHTMARE_ECTOPLASM, 105.102, 227.211, -102.752, 0, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 15000);
                                me->SummonCreature(NPC_NIGHTMARE_ECTOPLASM, 153.372, 235.149, -102.826, 0, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 15000);
                                me->SummonCreature(NPC_NIGHTMARE_ECTOPLASM, 149.524, 251.113, -102.558, 0, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 15000);
                                me->SummonCreature(NPC_NIGHTMARE_ECTOPLASM, 136.208, 266.466, -102.977, 0, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 15000);
                                me->SummonCreature(NPC_NIGHTMARE_ECTOPLASM, 126.167, 274.759, -102.962, 0, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 15000);
                            }
                            else
                            if (eventProgress == 5)
                            {
                                ++eventProgress;
                                if (Creature* naralex = pInstance->instance->GetCreature(pInstance->GetData64(DATA_NARALEX)))
                                    DoScriptText(EMOTE_HORRENDOUS_VISION, naralex);
                                me->SummonCreature(NPC_MUTANUS_THE_DEVOURER, 150.872, 262.905, -103.503, 0, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 300000);
                                DoScriptText(SAY_MUTANUS_THE_DEVOURER, me);
                                pInstance->SetData(TYPE_MUTANUS_THE_DEVOURER, IN_PROGRESS);
                            }
                            else
                            if (eventProgress == 6 && pInstance->GetData(TYPE_MUTANUS_THE_DEVOURER) == DONE)
                            {
                                ++eventProgress;
                                eventTimer = 3000;
                                if (Creature* naralex = pInstance->instance->GetCreature(pInstance->GetData64(DATA_NARALEX)))
                                {
                                    if (me->HasAuraEffect(SPELL_NARALEXS_AWAKENING))
                                        me->RemoveAura(SPELL_NARALEXS_AWAKENING, 0);
                                    naralex->SetStandState(UNIT_STAND_STATE_STAND);
                                    DoScriptText(SAY_I_AM_AWAKE, naralex);
                                }
                                DoScriptText(SAY_NARALEX_AWAKES, me);
                            }
                            else
                            if (eventProgress == 7)
                            {
                                ++eventProgress;
                                eventTimer = 6000;
                                if (Creature* naralex = pInstance->instance->GetCreature(pInstance->GetData64(DATA_NARALEX)))
                                    DoScriptText(SAY_THANK_YOU, naralex);
                            }
                            else
                            if (eventProgress == 8)
                            {
                                ++eventProgress;
                                eventTimer = 8000;
                                if (Creature* naralex = pInstance->instance->GetCreature(pInstance->GetData64(DATA_NARALEX)))
                                {
                                    DoScriptText(SAY_FAREWELL, naralex);
                                    naralex->AddAura(SPELL_FLIGHT_FORM, naralex);
                                    naralex->SetDisableGravity(true);
                                }
                                SetRun();
                                me->SetStandState(UNIT_STAND_STATE_STAND);
                                me->AddAura(SPELL_FLIGHT_FORM, me);
                                me->SetDisableGravity(true);
                            }
                            else
                            if (eventProgress == 9)
                            {
                                ++eventProgress;
                                eventTimer = 1500;
                                if (Creature* naralex = pInstance->instance->GetCreature(pInstance->GetData64(DATA_NARALEX)))
                                    naralex->GetMotionMaster()->MovePoint(25, naralex->GetPositionX(), naralex->GetPositionY(), naralex->GetPositionZ());
                            }
                            else
                            if (eventProgress == 10)
                            {
                                ++eventProgress;
                                eventTimer = 2500;
                                if (Creature* naralex = pInstance->instance->GetCreature(pInstance->GetData64(DATA_NARALEX)))
                                {
                                    naralex->AddUnitState(UNIT_STATE_IGNORE_PATHFINDING);
                                    naralex->GetMotionMaster()->MovePoint(0, 117.095512, 247.107971, -96.167870);
                                    naralex->GetMotionMaster()->MovePoint(1, 90.388809, 276.135406, -83.389801);
                                }
                                me->AddUnitState(UNIT_STATE_IGNORE_PATHFINDING);
                                me->GetMotionMaster()->MovePoint(26, 117.095512, 247.107971, -96.167870);
                                me->GetMotionMaster()->MovePoint(27, 144.375443, 281.045837, -82.477135);
                            }
                            else
                            if (eventProgress == 11)
                            {
                                if (Creature* naralex = pInstance->instance->GetCreature(pInstance->GetData64(DATA_NARALEX)))
                                    naralex->SetVisible(false);
                                me->SetVisible(false);
                                pInstance->SetData(TYPE_NARALEX_PART3, DONE);
                            }
                        break;
                    }
                }
            } else eventTimer -= diff;
        }

        virtual bool GossipHello(Player* pPlayer) override
        {
            if (pInstance)
            {
                me->CastSpell(pPlayer, SPELL_MARK_OF_THE_WILD_RANK_2, TRIGGERED_FULL_MASK);
                if ((pInstance->GetData(TYPE_LORD_COBRAHN) == DONE) && (pInstance->GetData(TYPE_LORD_PYTHAS) == DONE) &&
                    (pInstance->GetData(TYPE_LADY_ANACONDRA) == DONE) && (pInstance->GetData(TYPE_LORD_SERPENTIS) == DONE))
                {
                    pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_NARALEX, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
                    pPlayer->SEND_GOSSIP_MENU_TEXTID(GOSSIP_ID_START_2, me->GetGUID());

                    if (!pInstance->GetData(TYPE_NARALEX_YELLED))
                    {
                        DoScriptText(SAY_AT_LAST, me);
                        pInstance->SetData(TYPE_NARALEX_YELLED, 1);
                    }
                }
                else
                {
                    pPlayer->SEND_GOSSIP_MENU_TEXTID(GOSSIP_ID_START_1, me->GetGUID());
                }
            }
            return true;

        }


        virtual bool GossipSelect(Player* player, uint32 menuId, uint32 gossipListId) override
        {
            uint32 const action = player->PlayerTalkClass->GetGossipOptionAction(gossipListId);
            if (action == GOSSIP_ACTION_INFO_DEF + 1)
            {
                player->CLOSE_GOSSIP_MENU();
                if (pInstance)
                    pInstance->SetData(TYPE_NARALEX_EVENT, IN_PROGRESS);

                DoScriptText(SAY_MAKE_PREPARATIONS, me);

                me->SetFaction(FACTION_ESCORTEE_N_NEUTRAL_ACTIVE);
                me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC);
                me->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);

                CAST_AI(EscortAI, (me->AI()))->Start(true, false, player->GetGUID());
                CAST_AI(EscortAI, (me->AI()))->SetDespawnAtFar(false);
                CAST_AI(EscortAI, (me->AI()))->SetDespawnAtEnd(false);
            }
            return true;

        }

    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_disciple_of_naralexAI(creature);
    }
};




void AddSC_wailing_caverns()
{

    new npc_disciple_of_naralex();
}