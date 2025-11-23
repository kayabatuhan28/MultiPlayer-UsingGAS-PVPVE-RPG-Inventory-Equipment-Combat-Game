# MultiPlayer-UsingGAS-PVPVE-RPG-Inventory-Equipment-Combat-Game
Gameplay Video Link : https://drive.google.com/file/d/1N9JLCH5Vhx6qkWeEXr9AAMCSnaBUbRvI/view?usp=drive_link 
  
   

------------------------------------------------------------ TR ------------------------------------------------------------

Projenin Ana İçeriği: Co-op şekilde oyuncular temel statlarla ve basic bir clubla oyuna başlamaktalar. Farklı soulsLike tarzda zorluktaki yapay zekalarla çeşitli combat mekanikleriyle Kamp temizleye temizleye ilerleyip daha güçlü silahlar, zirhlar, takilar düşürüp, level alıp yapacakları builde göre item dizerek bir sonraki kampi temizlemeye çalismaktalar. Kullandıkları silahın skill treesindeki skilleri geliştirerek (Aynı anda birden çok silahtaki skillde geliştirilebilir) kendilerini güçlendirip en son bossu kesmeye gidecek şekilde bir ilerleyiş mevcut.

Proje öne çıkan özellikleri (tamamen multiplayer odaklı tasarlandı):
- Envanter sistemi fastArray yapisi kullanilarak oluşturuldu, Widgetlarda composite pattern kullanıldı.
- Projede envanter sistemi, Equipment sistemi, SkillTree sistemi, combat, Ai gibi birden çok sistem mevcut.
- Projenin neredeyse tamamı C++ ile geliştirildi ve Gameplay Ability System (GAS) üzerine kuruldu.
- Yapay zekâ, klasik MMORPG’lerdeki öngörülebilir, sırayla skill atan, kolay kesilebilir düşmanlar yerine Souls-like tarzı bir dövüş mantığına sahip.
- 6 farklı silah türü bulunuyor ve her birinde 4 özel skill mevcut. Bu skill’ler bir skill tree sistemi üzerinden açılabiliyor ve geliştirilebiliyor. Skiller haricinde combolu saldırı, Heavy Attack veya RMB ile guard alma, sürekli flame throw atma, ileri geri dodge sonrası özel saldırı yapma vs. Gibi silah türüne özel farklı combat mekanikleri mevcut
- Toplamda 11 ekipman/takı slotu yer alıyor: 6 zırh parçası, 2 yüzük, 1 kolye, 1 pelerin ve 1 silah.
- Eşyalar 4 farklı nadirlik seviyesinde (Common, Rare, Epic, Legendary) geliyor. Donatılan zırh, silah, takı, kolye vs. nadirlik durumuna göre özellikler ve alt statlar dinamik olarak değişiyor.
- Çok sayıda yüzük, kolye ve zırh parçası mevcut ancak videoda hepsini tek tek göstermek yerine oynanış sırasında düşen bazı loot’lar üzerinden örnekler paylaştım.
- Videonun son 5 dakikasında bir boss savaşı ve PvP düelloları yer alırken, önceki kısımlarda farklı türde yapay zekâ düşmanlarla savaşıp kamp temizleme sahneleri bulunuyor. 

------------------------------------------------------------ EN ------------------------------------------------------------

Project Gameplay Story: In this co-op experience, players start the game with basic stats and a simple club. They progress by clearing camps filled with AI enemies designed with Soulslike difficulty and various combat mechanics. As they fight through these camps, players obtain stronger weapons, armor pieces, accessories, and level up. Depending on the build they want to create, they equip and arrange items accordingly before moving on to the next camp. By upgrading the skills in the weapon’s skill tree (multiple weapon skill trees can be upgraded simultaneously), players grow stronger and eventually head out to defeat the final boss.

- Highlights of the Project (Fully Designed With Multiplayer Focus):
- The inventory system is built using a FastArray structure, and Composite Pattern is used for widgets.
- The project includes multiple systems such as Inventory, Equipment, Skill Tree, Combat, AI, etc.
- Almost the entire project is developed in C++ and built on top of the Gameplay Ability System (GAS).
- The AI is designed with a Soulslike combat mentality, unlike classic MMORPG enemies that act predictably, rotate abilities in order, and are easy to defeat.
- There are 6 different weapon types, each offering 4 unique skills. These skills can be unlocked and upgraded through a dedicated skill tree.
- Beyond skills, each weapon type also has its own combat mechanics combo attacks, heavy attacks, guarding with RMB, continuous flamethrower usage, special moves after forward/backward dodges, and more.
- The game contains 11 equipment/accessory slots in total: 6 armor pieces, 2 rings, 1 necklace, 1 cape, and 1 weapon.
- Items come in 4 rarity tiers (Common, Rare, Epic, Legendary). Depending on the rarity, equipped items such as armor, weapons, rings, or necklaces dynamically change their stats and sub-attributes.
- There are many rings, necklaces, and armor pieces available, but instead of showing every single item, the video showcases selected loot drops encountered during gameplay.
- The last 5 minutes of the video feature a boss fight and PvP duels, while earlier sections show camp-clearing sequences and combat encounters with various AI enemy types.


