/* Copyright (C) 2006 - 2008 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 */

/* ScriptData
SDName: Boss_Mother_Smolderweb
SD%Complete: 100
SDComment: Uncertain how often mother's milk is casted
SDCategory: Blackrock Spire
EndScriptData */



#define SPELL_CRYSTALIZE                16104
#define SPELL_MOTHERSMILK               16468
#define SPELL_SUMMON_SPIRE_SPIDERLING   16103

class boss_mother_smolderweb : public CreatureScript
{
public:
    boss_mother_smolderweb() : CreatureScript("boss_mother_smolderweb")
    { }

    class boss_mothersmolderwebAI : public ScriptedAI
    {
        public:
        boss_mothersmolderwebAI(Creature *c) : ScriptedAI(c) {}
    
        uint32 Crystalize_Timer;
        uint32 MothersMilk_Timer;
    
        void Reset()
        override {
            Crystalize_Timer = 20000;
            MothersMilk_Timer = 10000;
        }
    
        void EnterCombat(Unit *who) override { }
    
        void DamageTaken(Unit *done_by, uint32 &damage)
        override {
            if( me->GetHealth() <= damage )
                me->CastSpell(me,SPELL_SUMMON_SPIRE_SPIDERLING,true);
        }
    
        void UpdateAI(const uint32 diff)
        override {
            //Return since we have no target
            if (!UpdateVictim() )
                return;
    
            //Crystalize_Timer
            if( Crystalize_Timer < diff )
            {
                DoCast(me,SPELL_CRYSTALIZE);
                Crystalize_Timer = 15000;
            }else Crystalize_Timer -= diff;
    
            //MothersMilk_Timer
            if( MothersMilk_Timer < diff )
            {
                DoCast(me,SPELL_MOTHERSMILK);
                MothersMilk_Timer = 5000+rand()%7500;
            }else MothersMilk_Timer -= diff;
    
            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new boss_mothersmolderwebAI(creature);
    }
};


void AddSC_boss_mothersmolderweb()
{
    new boss_mother_smolderweb();
}
