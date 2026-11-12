// include/lib/texcache_templates.h
// The library template code src/lib/src/graphics/texcache.cpp instantiates: VC5's std::map and
// std::list for the texture cache (key std::pair<GfxTexHeader *, unsigned int>, value
// LPDIRECTDRAWSURFACE). The SP3 front end emits the 19 functions as COMDATs of TexCache.cpp's object,
// in retail's order, right after the cache's own code (0x0069da10..0x0069f910). Their bodies are
// <xtree>, <list> and <utility>; each is identified by its decorated name. Nothing includes this
// file.

#ifndef LIB_TEXCACHE_TEMPLATES_H
#define LIB_TEXCACHE_TEMPLATES_H

// std::map::~map with ~_Tree inlined (the cache's at-exit function $E6 at 0x0069cc0d is its only caller):
// erases the elements, frees the head node, clears head and size, and decrements the locked shared
// Nil reference count, freeing Nil when the count reaches zero.
// TEMPLATE: FF7 0x0069da10 SYMBOL
// ??1?$map@U?$pair@PAUGfxTexHeader@@I@std@@PAUIDirectDrawSurface@@U?$less@U?$pair@PAUGfxTexHeader@@I@std@@@2@V?$allocator@PAUIDirectDrawSurface@@@2@@std@@QAE@XZ

// The std::map constructor with a comparator and an allocator: disables duplicate keys, initializes the
// shared Nil node under the runtime lock, and allocates an empty red head node with size zero.
// TEMPLATE: FF7 0x0069dad0 SYMBOL
// ??0?$map@U?$pair@PAUGfxTexHeader@@I@std@@PAUIDirectDrawSurface@@U?$less@U?$pair@PAUGfxTexHeader@@I@std@@@2@V?$allocator@PAUIDirectDrawSurface@@@2@@std@@QAE@ABU?$less@U?$pair@PAUGfxTexHeader@@I@std@@@1@ABV?$allocator@PAUIDirectDrawSurface@@@1@@Z

// std::list<Cache::iterator>::~list: erases every element, frees the sentinel node, and clears the head
// pointer and size.
// TEMPLATE: FF7 0x0069dc30 SYMBOL
// ??1?$list@Viterator@?$_Tree@U?$pair@PAUGfxTexHeader@@I@std@@U?$pair@U?$pair@PAUGfxTexHeader@@I@std@@PAUIDirectDrawSurface@@@2@U_Kfn@?$map@U?$pair@PAUGfxTexHeader@@I@std@@PAUIDirectDrawSurface@@U?$less@U?$pair@PAUGfxTexHeader@@I@std@@@2@V?$allocator@PAUIDirectDrawSurface@@@2@@2@U?$less@U?$pair@PAUGfxTexHeader@@I@std@@@2@V?$allocator@PAUIDirectDrawSurface@@@2@@std@@V?$allocator@Viterator@?$_Tree@U?$pair@PAUGfxTexHeader@@I@std@@U?$pair@U?$pair@PAUGfxTexHeader@@I@std@@PAUIDirectDrawSurface@@@2@U_Kfn@?$map@U?$pair@PAUGfxTexHeader@@I@std@@PAUIDirectDrawSurface@@U?$less@U?$pair@PAUGfxTexHeader@@I@std@@@2@V?$allocator@PAUIDirectDrawSurface@@@2@@2@U?$less@U?$pair@PAUGfxTexHeader@@I@std@@@2@V?$allocator@PAUIDirectDrawSurface@@@2@@std@@@3@@std@@QAE@XZ

// std::_Tree::begin (map::begin forwards to it): the iterator at the head's leftmost node; for an empty
// tree, the head (end) node.
// TEMPLATE: FF7 0x0069dd40 SYMBOL
// ?begin@?$_Tree@U?$pair@PAUGfxTexHeader@@I@std@@U?$pair@U?$pair@PAUGfxTexHeader@@I@std@@PAUIDirectDrawSurface@@@2@U_Kfn@?$map@U?$pair@PAUGfxTexHeader@@I@std@@PAUIDirectDrawSurface@@U?$less@U?$pair@PAUGfxTexHeader@@I@std@@@2@V?$allocator@PAUIDirectDrawSurface@@@2@@2@U?$less@U?$pair@PAUGfxTexHeader@@I@std@@@2@V?$allocator@PAUIDirectDrawSurface@@@2@@std@@QAE?AViterator@12@XZ

// std::_Tree::insert(const value_type &), the unique insert: walks down from the root comparing keys,
// then inserts unless the key is already present. Its key comparisons expand the library's pair
// operator< inline in the loop and call it after --_P; the last two returns call the out-of-line
// pair<iterator, bool> constructor.
// TEMPLATE: FF7 0x0069dd70 SYMBOL
// ?insert@?$_Tree@U?$pair@PAUGfxTexHeader@@I@std@@U?$pair@U?$pair@PAUGfxTexHeader@@I@std@@PAUIDirectDrawSurface@@@2@U_Kfn@?$map@U?$pair@PAUGfxTexHeader@@I@std@@PAUIDirectDrawSurface@@U?$less@U?$pair@PAUGfxTexHeader@@I@std@@@2@V?$allocator@PAUIDirectDrawSurface@@@2@@2@U?$less@U?$pair@PAUGfxTexHeader@@I@std@@@2@V?$allocator@PAUIDirectDrawSurface@@@2@@std@@QAE?AU?$pair@Viterator@?$_Tree@U?$pair@PAUGfxTexHeader@@I@std@@U?$pair@U?$pair@PAUGfxTexHeader@@I@std@@PAUIDirectDrawSurface@@@2@U_Kfn@?$map@U?$pair@PAUGfxTexHeader@@I@std@@PAUIDirectDrawSurface@@U?$less@U?$pair@PAUGfxTexHeader@@I@std@@@2@V?$allocator@PAUIDirectDrawSurface@@@2@@2@U?$less@U?$pair@PAUGfxTexHeader@@I@std@@@2@V?$allocator@PAUIDirectDrawSurface@@@2@@std@@_N@2@ABU?$pair@U?$pair@PAUGfxTexHeader@@I@std@@PAUIDirectDrawSurface@@@2@@Z

// std::_Tree::erase(iterator): unlinks the node, reconnects or substitutes its successor, restores the
// red-black balance, destroys and frees the node, decrements the size and returns the next iterator,
// under the runtime lock.
// TEMPLATE: FF7 0x0069df60 SYMBOL
// ?erase@?$_Tree@U?$pair@PAUGfxTexHeader@@I@std@@U?$pair@U?$pair@PAUGfxTexHeader@@I@std@@PAUIDirectDrawSurface@@@2@U_Kfn@?$map@U?$pair@PAUGfxTexHeader@@I@std@@PAUIDirectDrawSurface@@U?$less@U?$pair@PAUGfxTexHeader@@I@std@@@2@V?$allocator@PAUIDirectDrawSurface@@@2@@2@U?$less@U?$pair@PAUGfxTexHeader@@I@std@@@2@V?$allocator@PAUIDirectDrawSurface@@@2@@std@@QAE?AViterator@12@V312@@Z

// std::_Tree::erase(iterator, iterator): for the whole nonempty range releases the tree recursively,
// resets root and size and points both extremes at the head; otherwise erases one node at a time.
// Returns the end iterator.
// TEMPLATE: FF7 0x0069eab0 SYMBOL
// ?erase@?$_Tree@U?$pair@PAUGfxTexHeader@@I@std@@U?$pair@U?$pair@PAUGfxTexHeader@@I@std@@PAUIDirectDrawSurface@@@2@U_Kfn@?$map@U?$pair@PAUGfxTexHeader@@I@std@@PAUIDirectDrawSurface@@U?$less@U?$pair@PAUGfxTexHeader@@I@std@@@2@V?$allocator@PAUIDirectDrawSurface@@@2@@2@U?$less@U?$pair@PAUGfxTexHeader@@I@std@@@2@V?$allocator@PAUIDirectDrawSurface@@@2@@std@@QAE?AViterator@12@V312@0@Z

// std::_Tree::iterator::_Inc: the in-order successor (the right subtree's minimum or up the parent
// links), reading the shared Nil node under the runtime lock.
// TEMPLATE: FF7 0x0069ec80 SYMBOL
// ?_Inc@iterator@?$_Tree@U?$pair@PAUGfxTexHeader@@I@std@@U?$pair@U?$pair@PAUGfxTexHeader@@I@std@@PAUIDirectDrawSurface@@@2@U_Kfn@?$map@U?$pair@PAUGfxTexHeader@@I@std@@PAUIDirectDrawSurface@@U?$less@U?$pair@PAUGfxTexHeader@@I@std@@@2@V?$allocator@PAUIDirectDrawSurface@@@2@@2@U?$less@U?$pair@PAUGfxTexHeader@@I@std@@@2@V?$allocator@PAUIDirectDrawSurface@@@2@@std@@QAEXXZ

// The out-of-line pair<iterator, bool> constructor: copies the iterator and the flag through their
// references.
// TEMPLATE: FF7 0x0069ed60 SYMBOL
// ??0?$pair@Viterator@?$_Tree@U?$pair@PAUGfxTexHeader@@I@std@@U?$pair@U?$pair@PAUGfxTexHeader@@I@std@@PAUIDirectDrawSurface@@@2@U_Kfn@?$map@U?$pair@PAUGfxTexHeader@@I@std@@PAUIDirectDrawSurface@@U?$less@U?$pair@PAUGfxTexHeader@@I@std@@@2@V?$allocator@PAUIDirectDrawSurface@@@2@@2@U?$less@U?$pair@PAUGfxTexHeader@@I@std@@@2@V?$allocator@PAUIDirectDrawSurface@@@2@@std@@_N@std@@QAE@ABViterator@?$_Tree@U?$pair@PAUGfxTexHeader@@I@std@@U?$pair@U?$pair@PAUGfxTexHeader@@I@std@@PAUIDirectDrawSurface@@@2@U_Kfn@?$map@U?$pair@PAUGfxTexHeader@@I@std@@PAUIDirectDrawSurface@@U?$less@U?$pair@PAUGfxTexHeader@@I@std@@@2@V?$allocator@PAUIDirectDrawSurface@@@2@@2@U?$less@U?$pair@PAUGfxTexHeader@@I@std@@@2@V?$allocator@PAUIDirectDrawSurface@@@2@@1@AB_N@Z

// std::list<Cache::iterator>::insert(iterator, const value_type &): links a new node before the
// iterator, copies the value, increments the size and returns the new node's iterator.
// TEMPLATE: FF7 0x0069ed90 SYMBOL
// ?insert@?$list@Viterator@?$_Tree@U?$pair@PAUGfxTexHeader@@I@std@@U?$pair@U?$pair@PAUGfxTexHeader@@I@std@@PAUIDirectDrawSurface@@@2@U_Kfn@?$map@U?$pair@PAUGfxTexHeader@@I@std@@PAUIDirectDrawSurface@@U?$less@U?$pair@PAUGfxTexHeader@@I@std@@@2@V?$allocator@PAUIDirectDrawSurface@@@2@@2@U?$less@U?$pair@PAUGfxTexHeader@@I@std@@@2@V?$allocator@PAUIDirectDrawSurface@@@2@@std@@V?$allocator@Viterator@?$_Tree@U?$pair@PAUGfxTexHeader@@I@std@@U?$pair@U?$pair@PAUGfxTexHeader@@I@std@@PAUIDirectDrawSurface@@@2@U_Kfn@?$map@U?$pair@PAUGfxTexHeader@@I@std@@PAUIDirectDrawSurface@@U?$less@U?$pair@PAUGfxTexHeader@@I@std@@@2@V?$allocator@PAUIDirectDrawSurface@@@2@@2@U?$less@U?$pair@PAUGfxTexHeader@@I@std@@@2@V?$allocator@PAUIDirectDrawSurface@@@2@@std@@@3@@std@@QAE?AViterator@12@V312@ABV3?$_Tree@U?$pair@PAUGfxTexHeader@@I@std@@U?$pair@U?$pair@PAUGfxTexHeader@@I@std@@PAUIDirectDrawSurface@@@2@U_Kfn@?$map@U?$pair@PAUGfxTexHeader@@I@std@@PAUIDirectDrawSurface@@U?$less@U?$pair@PAUGfxTexHeader@@I@std@@@2@V?$allocator@PAUIDirectDrawSurface@@@2@@2@U?$less@U?$pair@PAUGfxTexHeader@@I@std@@@2@V?$allocator@PAUIDirectDrawSurface@@@2@@2@@Z

// std::list<Cache::iterator>::erase(iterator, iterator): erases from the first iterator up to the
// second and returns it.
// TEMPLATE: FF7 0x0069ee80 SYMBOL
// ?erase@?$list@Viterator@?$_Tree@U?$pair@PAUGfxTexHeader@@I@std@@U?$pair@U?$pair@PAUGfxTexHeader@@I@std@@PAUIDirectDrawSurface@@@2@U_Kfn@?$map@U?$pair@PAUGfxTexHeader@@I@std@@PAUIDirectDrawSurface@@U?$less@U?$pair@PAUGfxTexHeader@@I@std@@@2@V?$allocator@PAUIDirectDrawSurface@@@2@@2@U?$less@U?$pair@PAUGfxTexHeader@@I@std@@@2@V?$allocator@PAUIDirectDrawSurface@@@2@@std@@V?$allocator@Viterator@?$_Tree@U?$pair@PAUGfxTexHeader@@I@std@@U?$pair@U?$pair@PAUGfxTexHeader@@I@std@@PAUIDirectDrawSurface@@@2@U_Kfn@?$map@U?$pair@PAUGfxTexHeader@@I@std@@PAUIDirectDrawSurface@@U?$less@U?$pair@PAUGfxTexHeader@@I@std@@@2@V?$allocator@PAUIDirectDrawSurface@@@2@@2@U?$less@U?$pair@PAUGfxTexHeader@@I@std@@@2@V?$allocator@PAUIDirectDrawSurface@@@2@@std@@@3@@std@@QAE?AViterator@12@V312@0@Z

// std::list<Cache::iterator>::_Buynode: allocates one 12-byte node and sets its next and previous links,
// using the node itself for each null argument.
// TEMPLATE: FF7 0x0069ef40 SYMBOL
// ?_Buynode@?$list@Viterator@?$_Tree@U?$pair@PAUGfxTexHeader@@I@std@@U?$pair@U?$pair@PAUGfxTexHeader@@I@std@@PAUIDirectDrawSurface@@@2@U_Kfn@?$map@U?$pair@PAUGfxTexHeader@@I@std@@PAUIDirectDrawSurface@@U?$less@U?$pair@PAUGfxTexHeader@@I@std@@@2@V?$allocator@PAUIDirectDrawSurface@@@2@@2@U?$less@U?$pair@PAUGfxTexHeader@@I@std@@@2@V?$allocator@PAUIDirectDrawSurface@@@2@@std@@V?$allocator@Viterator@?$_Tree@U?$pair@PAUGfxTexHeader@@I@std@@U?$pair@U?$pair@PAUGfxTexHeader@@I@std@@PAUIDirectDrawSurface@@@2@U_Kfn@?$map@U?$pair@PAUGfxTexHeader@@I@std@@PAUIDirectDrawSurface@@U?$less@U?$pair@PAUGfxTexHeader@@I@std@@@2@V?$allocator@PAUIDirectDrawSurface@@@2@@2@U?$less@U?$pair@PAUGfxTexHeader@@I@std@@@2@V?$allocator@PAUIDirectDrawSurface@@@2@@std@@@3@@std@@IAEPAU_Node@12@PAU312@0@Z

// std::_Tree::_Erase: releases a subtree recursively (each right subtree, then down the left chain),
// destroying each value and freeing its node, stopping at the shared Nil node under the runtime lock.
// TEMPLATE: FF7 0x0069efc0 SYMBOL
// ?_Erase@?$_Tree@U?$pair@PAUGfxTexHeader@@I@std@@U?$pair@U?$pair@PAUGfxTexHeader@@I@std@@PAUIDirectDrawSurface@@@2@U_Kfn@?$map@U?$pair@PAUGfxTexHeader@@I@std@@PAUIDirectDrawSurface@@U?$less@U?$pair@PAUGfxTexHeader@@I@std@@@2@V?$allocator@PAUIDirectDrawSurface@@@2@@2@U?$less@U?$pair@PAUGfxTexHeader@@I@std@@@2@V?$allocator@PAUIDirectDrawSurface@@@2@@std@@IAEXPAU_Node@12@@Z

// std::_Tree::_Insert: allocates and links a red node with the value, updates the size and the extreme
// links, then recolors and rotates the ancestors to restore the balance and blackens the root.
// Returns the new node's iterator.
// TEMPLATE: FF7 0x0069f050 SYMBOL
// ?_Insert@?$_Tree@U?$pair@PAUGfxTexHeader@@I@std@@U?$pair@U?$pair@PAUGfxTexHeader@@I@std@@PAUIDirectDrawSurface@@@2@U_Kfn@?$map@U?$pair@PAUGfxTexHeader@@I@std@@PAUIDirectDrawSurface@@U?$less@U?$pair@PAUGfxTexHeader@@I@std@@@2@V?$allocator@PAUIDirectDrawSurface@@@2@@2@U?$less@U?$pair@PAUGfxTexHeader@@I@std@@@2@V?$allocator@PAUIDirectDrawSurface@@@2@@std@@IAE?AViterator@12@PAU_Node@12@0ABU?$pair@U?$pair@PAUGfxTexHeader@@I@std@@PAUIDirectDrawSurface@@@2@@Z

// std::list<Cache::iterator>::erase(iterator): unlinks one node, destroys the value, frees the node,
// decrements the size and returns the next iterator.
// TEMPLATE: FF7 0x0069f660 SYMBOL
// ?erase@?$list@Viterator@?$_Tree@U?$pair@PAUGfxTexHeader@@I@std@@U?$pair@U?$pair@PAUGfxTexHeader@@I@std@@PAUIDirectDrawSurface@@@2@U_Kfn@?$map@U?$pair@PAUGfxTexHeader@@I@std@@PAUIDirectDrawSurface@@U?$less@U?$pair@PAUGfxTexHeader@@I@std@@@2@V?$allocator@PAUIDirectDrawSurface@@@2@@2@U?$less@U?$pair@PAUGfxTexHeader@@I@std@@@2@V?$allocator@PAUIDirectDrawSurface@@@2@@std@@V?$allocator@Viterator@?$_Tree@U?$pair@PAUGfxTexHeader@@I@std@@U?$pair@U?$pair@PAUGfxTexHeader@@I@std@@PAUIDirectDrawSurface@@@2@U_Kfn@?$map@U?$pair@PAUGfxTexHeader@@I@std@@PAUIDirectDrawSurface@@U?$less@U?$pair@PAUGfxTexHeader@@I@std@@@2@V?$allocator@PAUIDirectDrawSurface@@@2@@2@U?$less@U?$pair@PAUGfxTexHeader@@I@std@@@2@V?$allocator@PAUIDirectDrawSurface@@@2@@std@@@3@@std@@QAE?AViterator@12@V312@@Z

// std::list<Cache::iterator>::iterator::operator++(int): advances to the next node and returns the
// previous value.
// TEMPLATE: FF7 0x0069f6d0 SYMBOL
// ??Eiterator@?$list@Viterator@?$_Tree@U?$pair@PAUGfxTexHeader@@I@std@@U?$pair@U?$pair@PAUGfxTexHeader@@I@std@@PAUIDirectDrawSurface@@@2@U_Kfn@?$map@U?$pair@PAUGfxTexHeader@@I@std@@PAUIDirectDrawSurface@@U?$less@U?$pair@PAUGfxTexHeader@@I@std@@@2@V?$allocator@PAUIDirectDrawSurface@@@2@@2@U?$less@U?$pair@PAUGfxTexHeader@@I@std@@@2@V?$allocator@PAUIDirectDrawSurface@@@2@@std@@V?$allocator@Viterator@?$_Tree@U?$pair@PAUGfxTexHeader@@I@std@@U?$pair@U?$pair@PAUGfxTexHeader@@I@std@@PAUIDirectDrawSurface@@@2@U_Kfn@?$map@U?$pair@PAUGfxTexHeader@@I@std@@PAUIDirectDrawSurface@@U?$less@U?$pair@PAUGfxTexHeader@@I@std@@@2@V?$allocator@PAUIDirectDrawSurface@@@2@@2@U?$less@U?$pair@PAUGfxTexHeader@@I@std@@@2@V?$allocator@PAUIDirectDrawSurface@@@2@@std@@@3@@std@@QAE?AV012@H@Z

// std::_Tree::_Lbound: the first node whose key is not less than the requested key, or the head (end)
// node; walks down from the root to the shared Nil node.
// TEMPLATE: FF7 0x0069f710 SYMBOL
// ?_Lbound@?$_Tree@U?$pair@PAUGfxTexHeader@@I@std@@U?$pair@U?$pair@PAUGfxTexHeader@@I@std@@PAUIDirectDrawSurface@@@2@U_Kfn@?$map@U?$pair@PAUGfxTexHeader@@I@std@@PAUIDirectDrawSurface@@U?$less@U?$pair@PAUGfxTexHeader@@I@std@@@2@V?$allocator@PAUIDirectDrawSurface@@@2@@2@U?$less@U?$pair@PAUGfxTexHeader@@I@std@@@2@V?$allocator@PAUIDirectDrawSurface@@@2@@std@@IBEPAU_Node@12@ABU?$pair@PAUGfxTexHeader@@I@2@@Z

// std::_Tree::iterator::_Dec: the in-order predecessor; from the red head (end) node the rightmost node,
// otherwise the left subtree's maximum or up the parent links, under the runtime lock.
// TEMPLATE: FF7 0x0069f7c0 SYMBOL
// ?_Dec@iterator@?$_Tree@U?$pair@PAUGfxTexHeader@@I@std@@U?$pair@U?$pair@PAUGfxTexHeader@@I@std@@PAUIDirectDrawSurface@@@2@U_Kfn@?$map@U?$pair@PAUGfxTexHeader@@I@std@@PAUIDirectDrawSurface@@U?$less@U?$pair@PAUGfxTexHeader@@I@std@@@2@V?$allocator@PAUIDirectDrawSurface@@@2@@2@U?$less@U?$pair@PAUGfxTexHeader@@I@std@@@2@V?$allocator@PAUIDirectDrawSurface@@@2@@std@@QAEXXZ

// std::operator< for pairs (<utility>), the key order: header pointer, then palette. It is an ordinary
// inline function template, emitted after the template members because _Tree::insert calls it out of
// line once.
// TEMPLATE: FF7 0x0069f8c0 SYMBOL
// ??Mstd@@YA_NABU?$pair@PAUGfxTexHeader@@I@0@0@Z

// The tree's shared Nil node and its reference count (<xtree>), statics of _Tree that VC5 puts in
// COMDAT .bss after the cache's own. CodeView names longer than 255 characters are cut to 247 and
// end in eight hex digits of a hash; these lines are those names, as the PDB holds them.
// GLOBAL: FF7 0x00db2d10
// std::_Tree<std::pair<GfxTexHeader *,unsigned int>,std::pair<std::pair<GfxTexHeader *,unsigned int>,IDirectDrawSurface *>,std::map<std::pair<GfxTexHeader *,unsigned int>,IDirectDrawSurface *,std::less<std::pair<GfxTexHeader *,unsigned int> >,std::a676d316c

// GLOBAL: FF7 0x00db2d14
// std::_Tree<std::pair<GfxTexHeader *,unsigned int>,std::pair<std::pair<GfxTexHeader *,unsigned int>,IDirectDrawSurface *>,std::map<std::pair<GfxTexHeader *,unsigned int>,IDirectDrawSurface *,std::less<std::pair<GfxTexHeader *,unsigned int> >,std::a225dab76

#endif /* LIB_TEXCACHE_TEMPLATES_H */
