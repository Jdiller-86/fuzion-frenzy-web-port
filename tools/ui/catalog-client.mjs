const $ = id => document.getElementById(id);
const catalog = await (await fetch('catalog.json')).json();
const byId = new Map(catalog.resources.map(r => [r.stableId, r]));
const screenByResource = new Map();
for (const s of catalog.screens) for (const id of s.resources || []) { const a = screenByResource.get(id) || []; a.push(s); screenByResource.set(id, a); }
const esc = s => String(s ?? '').replace(/[&<>"']/g, c => ({'&':'&amp;','<':'&lt;','>':'&gt;','"':'&quot;',"'":'&#39;'}[c]));
function joinedScreens(r) {
 const j=r.joinedEvidence||{}; const raw=[...(Array.isArray(j.screens)?j.screens:[]),...(Array.isArray(j.screenReferences)?j.screenReferences:[]),...(Array.isArray(j.screenIds)?j.screenIds:[])];
 const base=screenByResource.get(r.stableId)||[]; const refs=raw.map(x=>typeof x==='string'?{name:x}:x).filter(x=>x&&typeof x==='object');
 const all=[...base,...refs]; return all.filter((x,i)=>all.findIndex(y=>(y.id||y.name||y.screenId)===(x.id||x.name||x.screenId))===i);
}
function confidenceValuesOf(r) {
 const j=r.joinedEvidence||{}; const vals=[j.confidence||j.relevance||r.relevance||'unclassified'];
 return [...new Set(vals.filter(Boolean).map(c=>typeof c==='string'?c:JSON.stringify(c)))];
}
function confidenceOf(r) { return confidenceValuesOf(r).join(', ')||'unclassified'; }
function regionsOf(r) { const j=r.joinedEvidence||{}; return r.atlasRegions?.length?r.atlasRegions:(Array.isArray(j.atlasRegions)?j.atlasRegions:Array.isArray(j.regions)?j.regions:[]); }
function options(id, values) { const node=$(id); for(const v of [...new Set(values.filter(Boolean).map(String))].sort()){const o=document.createElement('option');o.value=v;o.textContent=v;node.append(o);} }
options('package',catalog.resources.map(r=>r.package));
options('type',catalog.resources.flatMap(r=>[r.classification,r.joinedEvidence?.classification]));
options('screen',[...catalog.screens.map(s=>s.name||s.id),...catalog.resources.flatMap(r=>joinedScreens(r).map(s=>s.name||s.id||s.screenId))]);
options('confidence',catalog.resources.flatMap(confidenceValuesOf));
const filters=['search','package','type','screen','confidence','sort'];
function matches(r) {
 const q=$('search').value.trim().toLowerCase(), screens=joinedScreens(r), joined=r.joinedEvidence||{};
 const text=[r.stableId,r.resourceName,r.package,r.packagePath,r.format,r.classification,r.relevance,JSON.stringify(r.evidence||[]),JSON.stringify(joined),...screens.map(s=>s.name||s.id||s.screenId)].join(' ').toLowerCase();
 const type=r.joinedEvidence?.classification||r.classification;
 return (!q||text.includes(q))&&(!$('package').value||r.package===$('package').value)&&(!$('type').value||type===$('type').value)&&(!$('screen').value||screens.some(s=>(s.name||s.id||s.screenId)===$('screen').value))&&(!$('confidence').value||confidenceValuesOf(r).includes($('confidence').value));
}
function appendJson(parent,title,value) { const h=document.createElement('h3');h.textContent=title;parent.append(h);const details=document.createElement('details');const summary=document.createElement('summary');summary.textContent='Show full metadata';const pre=document.createElement('pre');pre.textContent=JSON.stringify(value,null,2);details.append(summary,pre);parent.append(details); }
function atlasSvg(regions,r) {
 const svg=document.createElementNS('http://www.w3.org/2000/svg','svg');svg.classList.add('atlas-overlay');
 const w=Number(r.width)||1,h=Number(r.height)||1;svg.setAttribute('viewBox',`0 0 ${w} ${h}`);svg.setAttribute('preserveAspectRatio','xMidYMid meet');
 const picture=document.createElementNS(svg.namespaceURI,'image'); picture.setAttribute('href',r.thumbnail||''); picture.setAttribute('width',w); picture.setAttribute('height',h); svg.append(picture);
 for(const [regionIndex,region] of regions.entries()){
   let box=Array.isArray(region.pixelRect)?region.pixelRect.map(Number):null;
   const uv=region.uvRect;
   if(!box&&Array.isArray(uv)&&uv.length>=4)box=uv.map((v,i)=>Number(v)*(i%2===0?w:h));
   if(!box&&uv&&typeof uv==='object'){
     const left=Number(uv.x??uv.u??uv.u0),top=Number(uv.y??uv.v??uv.v0);
     const right=Number(uv.u1),bottom=Number(uv.v1);
     const rw=Number(uv.width??(Number.isFinite(right)?right-left:NaN)),rh=Number(uv.height??(Number.isFinite(bottom)?bottom-top:NaN));
     if([left,top,rw,rh].every(Number.isFinite))box=[left*w,top*h,rw*w,rh*h];
   }
   if(!box||box.length<4||!box.every(Number.isFinite)||box[2]<=0||box[3]<=0)continue;
   const rect=document.createElementNS(svg.namespaceURI,'rect');rect.setAttribute('x',box[0]);rect.setAttribute('y',box[1]);rect.setAttribute('width',box[2]);rect.setAttribute('height',box[3]);rect.setAttribute('class','atlas-region');svg.append(rect);
   const text=document.createElementNS(svg.namespaceURI,'text');text.setAttribute('x',box[0]+w/100);text.setAttribute('y',box[1]+(regionIndex+1)*h/16);text.setAttribute('font-size',Math.max(w,h)/32);text.textContent='R'+(regionIndex+1);text.setAttribute('class','atlas-label');svg.append(text);
 }
 return svg;
}
function select(r) {
 const root=$('detail');root.classList.remove('empty');
 const frames=(r.frames||[]).map((f,i)=>`<figure><img src="${esc(f.thumbnail||r.thumbnail||'')}" alt="Frame ${i+1}"><figcaption>Frame ${esc(f.index??i)} · inventory order · ${esc(f.timing||'timing unknown')}</figcaption></figure>`).join('');
 const regions=regionsOf(r), known=regions.length>0, screens=joinedScreens(r), baseConfidence=confidenceOf(r);
 root.innerHTML=`<button class="close" aria-label="Close detail">×</button><h2>${esc(r.resourceName||'(unnamed resource)')}</h2><p><code>${esc(r.stableId)}</code></p><div class="full-preview"><img src="${esc(r.thumbnail||'')}" alt="Decoded resource ${esc(r.resourceName)}"></div><div id="atlasMount"></div><p>${esc(r.width??'?')} × ${esc(r.height??'?')} · ${esc(r.format||'format unknown')} · ${esc(r.joinedEvidence?.classification||r.classification||'unclassified')} · confidence ${esc(baseConfidence)}</p><h3>Source references</h3><dl><dt>Package path</dt><dd>${esc(r.packagePath||r.package||'unknown')}</dd><dt>Package</dt><dd>${esc(r.package||'unknown')}</dd><dt>Resource name</dt><dd>${esc(r.resourceName||'unknown')}</dd><dt>Source offset / size</dt><dd>${esc(r.offset??'unknown')} / ${esc(r.size??'unknown')}</dd><dt>XBP hash / SHA-256</dt><dd>${esc(r.xbpHash??'unknown')} / <code>${esc(r.sha256||'unknown')}</code></dd><dt>Alpha / mip levels</dt><dd>${esc(typeof r.alpha==='object'?JSON.stringify(r.alpha):r.alpha||'unknown')} / ${esc('header='+String(r.headerMipLevels??'unknown')+'; decoded chain='+String(r.mipChainDecoded??false))}</dd><dt>Decode status</dt><dd>${esc(r.decodeStatus||'unknown')}${r.decodeError?` · ${esc(r.decodeError)}`:''}</dd><dt>Screen associations</dt><dd>${screens.length?screens.map(s=>`${esc(s.name||s.id||s.screenId)} (${esc(s.confidence||s.status||'confidence not supplied')})`).join(', '):'None evidenced'}</dd></dl><h3>Geometry-derived texture rectangles</h3>${known?`<p>${esc(regions.length)} geometry-derived texture rectangle(s). Their semantic atlas membership is not established; a rectangle may cover an entire texture. Overlay labels identify geometry only.</p><p class="unknown">Coordinates come from isolated leaf parser output. Parent and sibling transforms are omitted; these are not global overlay coordinates.</p><ul>${regions.map(x=>`<li>${esc(x.label||x.name||'Texture rectangle')} · ${esc(x.semanticAtlasMembership||'atlas membership unknown')} · ${esc(JSON.stringify(x))}</li>`).join('')}</ul>`:'<p class="unknown">Texture rectangle geometry unknown; no overlay inferred.</p>'}<p><a href="${esc(r.thumbnail||'#')}" target="_blank" rel="noopener">Open full-resolution PNG (when decoded)</a></p><h3>Manual frame review</h3><div class="frames">${frames||'<p>No decoded frames supplied.</p>'}</div><p class="unknown">Frame order is inventory order only; playback timing is unknown unless evidenced.</p>`;
 const preview=root.querySelector('.full-preview'); if(known){const svg=atlasSvg(regions,r);preview.querySelector('img')?.remove();preview.append(svg);const legend=document.createElement('ol');regions.forEach((region,index)=>{const item=document.createElement('li');item.textContent='R'+(index+1)+': '+String(region.label||region.name||'geometry-derived rectangle')+' (atlas membership unknown)';legend.append(item);});root.querySelector('#atlasMount').append(legend);if(!svg.querySelector('rect'))root.querySelector('#atlasMount').innerHTML='<p class="unknown">Known regions are recorded, but no supported rectangle geometry was supplied for overlay.</p>';}
 appendJson(root,'Complete resource record',r);
 if(r.joinedEvidence)appendJson(root,'Joined code and screen evidence',r.joinedEvidence);
 if(screens.some(s=>s.joinedEvidence))appendJson(root,'Joined screen association evidence',screens.filter(s=>s.joinedEvidence).map(s=>({screen:s.name||s.id||s.screenId,evidence:s.joinedEvidence})));
 if(catalog.scope?.limits)appendJson(root,'Inventory limits',catalog.scope.limits);
 root.querySelector('.close').onclick=()=>root.classList.add('empty');
}
let page=0;const pageSize=80;
function render() {
 let list=catalog.resources.filter(matches);const sort=$('sort').value;
 list.sort((a,b)=>sort==='package'?String(a.package).localeCompare(String(b.package))||String(a.resourceName).localeCompare(String(b.resourceName)):sort==='size'?(b.width||0)*(b.height||0)-(a.width||0)*(a.height||0):String(a.resourceName||a.stableId).localeCompare(String(b.resourceName||b.stableId)));
 const pages=Math.max(1,Math.ceil(list.length/pageSize));page=Math.min(page,pages-1);const visible=list.slice(page*pageSize,(page+1)*pageSize);
 $('count').textContent=`${list.length.toLocaleString()} matching of ${catalog.resources.length.toLocaleString()} resources`;$('page').textContent=`Page ${page+1} of ${pages}`;$('previous').disabled=page===0;$('next').disabled=page>=pages-1;
 $('cards').innerHTML=visible.map(r=>`<button class="card" data-id="${esc(r.stableId)}"><span class="thumb">${r.thumbnail?`<img loading="lazy" src="${esc(r.thumbnail)}" alt="">`:'No thumbnail'}</span><b>${esc(r.resourceName||'(unnamed)')}</b><small>${esc(r.package||'package unknown')}</small><small>${esc(r.joinedEvidence?.classification||r.classification||'unclassified')} · ${esc(confidenceOf(r))} · ${esc(r.width??'?')}×${esc(r.height??'?')}</small></button>`).join('');
 for(const b of $('cards').querySelectorAll('[data-id]'))b.onclick=()=>select(byId.get(b.dataset.id));
}
for(const f of filters)$(f).addEventListener('input',()=>{page=0;render();});
$('previous').onclick=()=>{page=Math.max(0,page-1);render();};$('next').onclick=()=>{page++;render();};$('print').onclick=()=>window.print();
$('scope').textContent=`${catalog.scope?.resourceCount??catalog.resources.length} selected-candidate resources · complete-game coverage: ${catalog.scope?.completeGameInventory?'yes':'no'} · private local derivatives`;
const confidenceCounts=catalog.resources.reduce((a,r)=>{const c=r.joinedEvidence?.confidence||r.relevance||'unclassified';a[c]=(a[c]||0)+1;return a;},{});
$('totals').textContent=`${catalog.totals?.decodedResources??0} decoded · ${confidenceCounts.confirmed||0} confirmed · ${confidenceCounts.probable||0} probable · ${confidenceCounts.unclassified||0} unclassified · ${catalog.screens.filter(s=>s.evidenceKind).length} evidenced screens / ${catalog.screens.filter(s=>!s.evidenceKind).length} package leads`;
if(catalog.observations)$('observations').innerHTML=`<h3>Runtime observations</h3>${catalog.observations.map(o=>`<article class="observation"><b>${esc(o.id||'Observation')}</b><small>${esc(o.observedAt||'time unknown')} · ${esc(o.evidenceType||'evidence type unknown')}</small>${o.localCapture?`<a href="${esc(o.localCapture)}" target="_blank" rel="noopener"><img loading="lazy" src="${esc(o.localCapture)}" alt="Capture: ${esc(o.id||'observation')}"></a>`:'<p>Capture image unavailable.</p>'}<p>${esc(o.notes||'')}</p><small>${esc((o.limitations||[]).join(' · '))}</small></article>`).join('')}`;
render();
