// Device ownership and semantic input latching. Platform events/polling stay in the client.
export const KEYBOARD_LAYOUTS=Object.freeze([
  Object.freeze({jump:'Space',duck:'ArrowDown'}),Object.freeze({jump:'KeyW',duck:'KeyS'}),
  Object.freeze({jump:'KeyI',duck:'KeyK'}),Object.freeze({jump:'Numpad8',duck:'Numpad5'})
]);
const pressed=button=>!!button&&(button.pressed===true||(Number.isFinite(button.value)&&button.value>0.5));

export class LocalInput {
  constructor(seatCount=1){
    if(!Number.isInteger(seatCount)||seatCount<1||seatCount>4)throw new Error('Invalid seat count');
    this.bindings=Array.from({length:seatCount},(_,id)=>({type:'keyboard',...KEYBOARD_LAYOUTS[id]}));
    this.held=new Set();this.blockedKeys=new Set();this.pending=Array(seatCount).fill(false);
    this.padDuck=Array(seatCount).fill(false);this.padJump=Array(seatCount).fill(false);
    this.blockedPads=Array(seatCount).fill(true);this.present=Array(seatCount).fill(false);this.devices=[];
  }
  checkSeat(seat){if(!Number.isInteger(seat)||seat<0||seat>=this.bindings.length)throw new Error('Unknown seat');}
  assignKeyboard(seat,jump,duck){
    this.checkSeat(seat);
    if(typeof jump!=='string'||typeof duck!=='string'||jump===duck||![jump,duck].every(k=>/^(Key[A-Z]|Digit[0-9]|Numpad[0-9]|Space|Arrow(?:Up|Down|Left|Right))$/.test(k)))throw new Error('Choose two distinct gameplay keys');
    if(this.bindings.some((b,id)=>id!==seat&&b.type==='keyboard'&&[b.jump,b.duck].some(k=>k===jump||k===duck)))throw new Error('A key can belong to only one seat');
    this.bindings[seat]={type:'keyboard',jump,duck};this.clearSeat(seat);return this.bindings[seat];
  }
  assignGamepad(seat,index,id){
    this.checkSeat(seat);
    const device=this.devices.find(d=>d.index===index&&d.id===id&&d.standard);
    if(!device)throw new Error('Choose a connected standard gamepad');
    if(this.bindings.some((b,i)=>i!==seat&&b.type==='gamepad'&&b.index===index))throw new Error('A gamepad can belong to only one seat');
    this.bindings[seat]={type:'gamepad',index,id};this.clearSeat(seat);this.present[seat]=true;return this.bindings[seat];
  }
  clearSeat(seat){
    this.pending[seat]=false;this.padDuck[seat]=false;this.padJump[seat]=false;this.blockedPads[seat]=true;
    const binding=this.bindings[seat];
    if(binding.type==='keyboard')for(const code of [binding.jump,binding.duck]){if(this.held.has(code))this.blockedKeys.add(code);this.held.delete(code);}
  }
  clear(){
    for(const code of this.held)this.blockedKeys.add(code);
    this.held.clear();this.pending.fill(false);this.padDuck.fill(false);this.padJump.fill(false);this.blockedPads.fill(true);
  }
  keyDown(code,{repeat=false,editable=false}={}){
    if(editable)return false;
    const seat=this.bindings.findIndex(b=>b.type==='keyboard'&&(b.jump===code||b.duck===code));
    if(seat<0)return false;
    if(this.blockedKeys.has(code)||repeat||this.held.has(code))return true;
    this.held.add(code);if(this.bindings[seat].jump===code)this.pending[seat]=true;return true;
  }
  keyUp(code){this.held.delete(code);this.blockedKeys.delete(code);}
  pollGamepads(samples=[]){
    const pads=Array.from(samples??[]).filter(p=>p&&p.connected!==false&&Number.isInteger(p.index)&&p.index>=0&&p.index<32);
    this.devices=pads.map(p=>({index:p.index,id:String(p.id??'Gamepad').slice(0,160),standard:p.mapping==='standard'}));
    const lost=[];
    this.bindings.forEach((b,seat)=>{
      if(b.type!=='gamepad')return;
      const pad=pads.find(p=>p.index===b.index&&String(p.id??'Gamepad').slice(0,160)===b.id&&p.mapping==='standard');
      if(!pad){if(this.present[seat])lost.push({seat,index:b.index});this.present[seat]=false;this.clearSeat(seat);return;}
      this.present[seat]=true;
      const jump=pressed(pad.buttons?.[0]),duck=pressed(pad.buttons?.[1]);
      if(this.blockedPads[seat]){
        if(!jump&&!duck)this.blockedPads[seat]=false;
        this.padJump[seat]=jump;this.padDuck[seat]=false;return;
      }
      if(jump&&!this.padJump[seat])this.pending[seat]=true;
      this.padJump[seat]=jump;this.padDuck[seat]=duck;
    });
    return lost;
  }
  ready(){return this.bindings.every((b,seat)=>b.type==='keyboard'||this.present[seat]);}
  consume(){
    const commands=this.bindings.map((b,seat)=>({jumpPressed:this.pending[seat],duckHeld:b.type==='keyboard'?this.held.has(b.duck):this.padDuck[seat]}));
    this.pending.fill(false);return commands;
  }
}
