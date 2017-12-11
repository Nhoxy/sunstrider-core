/* ScriptData
SDName: Custom_Example
SD%Complete: 100
SDComment: Short custom scripting example
SDCategory: OLDScript Examples
EndScriptData */

#include "EscortAI.h"


// **** This script is designed as an example for others to build on ****
// **** Please modify whatever you'd like to as this script is only for developement ****

// **** OLDScript Info ****
// This script is written in a way that it can be used for both friendly and hostile monsters
// Its primary purpose is to show just how much you can really do with scripts
// I recommend trying it out on both an agressive NPC and on friendly npc

// **** Quick Info ****
// Functions with Handled Function marked above them are functions that are called automatically by the core
// Functions that are marked Custom Function are functions I've created to simplify code

#define SPELL_BUFF      25661
#define SPELL_ONE       12555
#define SPELL_ONE_ALT   24099
#define SPELL_TWO       10017
#define SPELL_THREE     26027
#define SPELL_ENRAGE    23537
#define SPELL_BESERK    32309

#define SAY_AGGRO       "Let the games begin."
#define SAY_RANDOM_0    "I see endless suffering. I see torment. I see rage. I see everything."
#define SAY_RANDOM_1    "Muahahahaha"
#define SAY_RANDOM_2    "These mortal infedels my lord, they have invaded your sanctum and seek to steal your secrets."
#define SAY_RANDOM_3    "You are already dead."
#define SAY_RANDOM_4    "Where to go? What to do? So many choices that all end in pain, end in death."
#define SAY_BESERK      "$N, I sentance you to death!"
#define SAY_PHASE       "The suffering has just begun!"

#define GOSSIP_ITEM     "I'm looking for a fight"
#define SAY_DANCE       "I always thought I was a good dancer"
#define SAY_SALUTE      "Move out Soldier!"


//This is the GetAI method used by all scripts that involve AI
//It is called every time a new creature using this script is created
class custom_example : public CreatureScript
{
public:
    custom_example() : CreatureScript("custom_example")
    { }

    class custom_exampleAI : public ScriptedAI
    {
        public:
        //*** HANDLED FUNCTION ***
        //This is the constructor, called only once when the creature is first created
        custom_exampleAI(Creature *c) : ScriptedAI(c) {}
    
        //*** CUSTOM VARIABLES ****
        //These variables are for use only by this individual script.
        //Nothing else will ever call them but us.
    
        uint32 Say_Timer;                                       //Timer for random chat
        uint32 Rebuff_Timer;                                    //Timer for rebuffing
        uint32 Spell_1_Timer;                                   //Timer for spell 1 when in combat
        uint32 Spell_2_Timer;                                   //Timer for spell 1 when in combat
        uint32 Spell_3_Timer;                                   //Timer for spell 1 when in combat
        uint32 Beserk_Timer;                                    //Timer until we go into Beserk (enraged) mode
        uint32 Phase;                                           //The current battle phase we are in
        uint32 Phase_Timer;                                     //Timer until phase transition
    
        //*** HANDLED FUNCTION ***
        //This is called whenever the core decides we need to evade
        void Reset()
        override {
            Phase = 1;                                          //Start in phase 1
            Phase_Timer = 60000;                                //60 seconds
            Spell_1_Timer = 5000;                               //5 seconds
            Spell_2_Timer = 37000;                              //37 seconds
            Spell_3_Timer = 19000;                              //19 seconds
            Beserk_Timer = 120000;                              //2 minutes
        }
    
        //*** HANDLED FUNCTION ***
        //Attack Start is called whenever someone hits us.
        void EnterCombat(Unit *who)
        override {
            //Say some stuff
            me->Say(SAY_AGGRO, LANG_UNIVERSAL);
            DoPlaySoundToSet(me,8280);
        }
    
        void ReceiveEmote(Player* player, uint32 emote) override
        {
            me->HandleEmoteCommand(emote);
    
            if (emote == TEXTEMOTE_DANCE)
                me->Say(SAY_DANCE);
    
            if (emote == TEXTEMOTE_SALUTE)
                me->Say(SAY_SALUTE);
        }
    
        //*** HANDLED FUNCTION ***
        //Update AI is called Every single map update (roughly once every 100ms if a player is within the grid)
        void UpdateAI(const uint32 diff)
        override {
            //Out of combat timers
            if (!me->GetVictim())
            {
                //Random Say timer
                if (Say_Timer < diff)
                {
                    //Random switch between 5 outcomes
                    switch (rand()%5)
                    {
                        case 0:
                            me->Yell(SAY_RANDOM_0,LANG_UNIVERSAL,nullptr);
                            DoPlaySoundToSet(me,8831);  //8831 is the index of the sound we are playing. You find these numbers in SoundEntries.dbc
                            break;
    
                        case 1:
                            me->Yell(SAY_RANDOM_1,LANG_UNIVERSAL,nullptr);
                            DoPlaySoundToSet(me,8818);
                            break;
    
                        case 2:
                            me->Yell(SAY_RANDOM_2,LANG_UNIVERSAL,nullptr);
                            DoPlaySoundToSet(me,8041);
                            break;
    
                        case 3:
                            me->Yell(SAY_RANDOM_3,LANG_UNIVERSAL,nullptr);
                            DoPlaySoundToSet(me,8581);
                            break;
    
                        case 4:
                            me->Yell(SAY_RANDOM_4,LANG_UNIVERSAL,nullptr);
                            DoPlaySoundToSet(me,8791);
                            break;
                    }
    
                    Say_Timer = 45000;                          //Say something agian in 45 seconds
                }else Say_Timer -= diff;
    
                //Rebuff timer
                if (Rebuff_Timer < diff)
                {
                    DoCast(me,SPELL_BUFF);
                    Rebuff_Timer = 900000;                      //Rebuff agian in 15 minutes
                }else Rebuff_Timer -= diff;
            }
    
            //Return since we have no target
            if (!UpdateVictim())
                return;
    
            //Spell 1 timer
            if (Spell_1_Timer < diff)
            {
                //Cast spell one on our current target.
                if (rand()%50 > 10)
                    DoCast(me->GetVictim(),SPELL_ONE_ALT);
                else if (me->GetDistance(me->GetVictim()) < 25)
                    DoCast(me->GetVictim(),SPELL_ONE);
    
                Spell_1_Timer = 5000;
            }else Spell_1_Timer -= diff;
    
            //Spell 2 timer
            if (Spell_2_Timer < diff)
            {
                //Cast spell one on our current target.
                DoCast(me->GetVictim(),SPELL_TWO);
    
                Spell_2_Timer = 37000;
            }else Spell_2_Timer -= diff;
    
            //Spell 3 timer
            if (Phase > 1)
            {
                if (Spell_3_Timer < diff)
                {
                    //Cast spell one on our current target.
                    DoCast(me->GetVictim(),SPELL_THREE);
    
                    Spell_3_Timer = 19000;
                } else { Spell_3_Timer -= diff; }
    
                //Beserk timer
                if (Beserk_Timer < diff)
                {
                    //Say our line then cast uber death spell
                    DoPlaySoundToSet(me,8588);
                    me->Yell(SAY_BESERK,LANG_UNIVERSAL,me->GetVictim());
                    DoCast(me->GetVictim(),SPELL_BESERK);
    
                    //Cast our beserk spell agian in 12 seconds if we didn't kill everyone
                    Beserk_Timer = 12000;
                }else Beserk_Timer -= diff;
            }
    
            //Phase timer
            if (Phase == 1)
            {
                if (Phase_Timer < diff)
                {
                    //Go to next phase
                    Phase++;
                    me->Yell(SAY_PHASE,LANG_UNIVERSAL,nullptr);
                    DoCast(me,SPELL_ENRAGE);
                }else Phase_Timer -= diff;
            }
    
            DoMeleeAttackIfReady();
        }

        virtual bool GossipHello(Player* player) override
        {
            player->ADD_GOSSIP_ITEM( GOSSIP_ICON_CHAT, GOSSIP_ITEM        , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
            player->SEND_GOSSIP_MENU_TEXTID(907,me->GetGUID());

            return true;

        }

        //This function is called when the player clicks an option on the gossip menu
        void SendDefaultMenu_custom_example(Player *player, Creature *_Creature, uint32 action)
        {
            if (action == GOSSIP_ACTION_INFO_DEF + 1)               //Fight time
            {
                //Set our faction to hostile twoards all
                _Creature->SetFaction(24);
                _Creature->Attack(player, true);
                player->PlayerTalkClass->SendCloseGossip();
            }
        }

        virtual bool GossipSelect(Player* player, uint32 menuId, uint32 gossipListId) override
        {
            uint32 const action = player->PlayerTalkClass->GetGossipOptionAction(gossipListId);
            uint32 const sender = player->PlayerTalkClass->GetGossipOptionSender(gossipListId);
            ClearGossipMenuFor(player);
            if (sender == GOSSIP_SENDER_MAIN)
                SendDefaultMenu_custom_example(player, me, action);

            return true;

        }

    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new custom_exampleAI(creature);
    }
};

//Meant to be spawn in map 13
class npc_escort_example : public CreatureScript
{
public:
    npc_escort_example() : CreatureScript("npc_escort_example")
    { }

    class npc_escort_exampleAI : public EscortAI
    {
    public:
        npc_escort_exampleAI(Creature *c) : EscortAI(c)
        {
            AddWaypoint(0, 67.0f, -70.0f, -144.0f);
            AddWaypoint(1, 107.0f, -70.0f, -144.0f);
            AddWaypoint(2, 107.0f, -100.0f, -144.0f);
            AddWaypoint(3, 67.0f, -100.0f, -144.0f);
        }

        void WaypointReached(uint32 i, uint32 pathID)
            override {

            switch (i)
            {
            case 0:
            default:
                std::stringstream str;
                str << "Reached wp " << i;
                me->Say(str.str().c_str());
                break;
            }
        }

        virtual bool GossipHello(Player* player) override
        {
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Start escort", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
            SEND_PREPARED_GOSSIP_MENU(player, me);
            return true;
        }

        virtual bool GossipSelect(Player* player, uint32 menuId, uint32 gossipListId) override
        {
            uint32 const action = player->PlayerTalkClass->GetGossipOptionAction(gossipListId);
            ClearGossipMenuFor(player);
            switch (action)
            {
            case GOSSIP_ACTION_INFO_DEF + 1:
                player->CLOSE_GOSSIP_MENU();
                ((EscortAI*)(me->AI()))->Start(true, false, player->GetGUID());
                ((EscortAI*)(me->AI()))->SetDespawnAtEnd(false);
                ((EscortAI*)(me->AI()))->SetDespawnAtFar(false);
                break;
            }
            return true;
        }

        void EnterCombat(Unit* who)
            override {}

        void Reset()
            override {}
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_escort_exampleAI(creature);
    }
};

//This is the actual function called only once durring InitScripts()
//It must define all handled functions that are to be run in this script
//For example if you want this OLDScript to handle Emotes you must include
//newscript->ReciveEmote = My_Emote_Function;
void AddSC_custom_example()
{
    new custom_example();
    new npc_escort_example();
}
