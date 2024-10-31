#ifndef QUAD_TREE_CLASS_H
#define QUAD_TREE_CLASS_H

#include <godot_cpp/classes/ref_counted.hpp>

using namespace godot;

class QuadTree : public RefCounted {
	GDCLASS(QuadTree, RefCounted);

public:
	class QuadRect : public RefCounted {
        GDCLASS(QuadRect, RefCounted);
    private:
		Rect2i rect;
		Variant metadata;
        
	protected:
		static void _bind_methods();

	public:
		void _init(const Rect2i &rect, const Variant &metadata);
        void set_rect(const Rect2i &new_rect);
	    Rect2i get_rect() const;
        void set_metadata(const Variant &new_metadata);
	    Variant get_metadata() const;
		String to_string() const;
	};

	class QuadNode : public RefCounted {
        GDCLASS(QuadNode, RefCounted);
	private:
		TypedArray<QuadNode> quadrants = TypedArray<QuadNode>();
		int quadrant_count = 0;
		TypedArray<QuadRect> quad_rects;
		Rect2i rect;
        
	protected:
		static void _bind_methods();

	public:
		QuadNode();
		~QuadNode();
		static bool can_subdivide(const Vector2i &size);
		static TypedArray<Rect2i> get_quadrant_rects(const Rect2i &rect);
		void _init(const Rect2i &rect);
        void set_quadrants(const TypedArray<QuadNode> &new_quadrants);
	    TypedArray<QuadNode> get_quadrants() const;
        void set_quadrant_count(const int &new_quadrant_count);
	    int get_quadrant_count() const;
        void set_quad_rects(const TypedArray<QuadRect> &new_quad_rects);
	    TypedArray<QuadRect> get_quad_rects() const;
        void set_rect(const Rect2i &new_rect);
	    Rect2i get_rect() const;
		String to_string() const;
		bool is_empty() const;
		Ref<QuadRect> get_first_under_rect(const Rect2i &test_rect, const Variant &exception_metadata = nullptr) const;
		Ref<QuadRect> get_first_containing_point(const Vector2i &point, const Variant &exception_metadata = nullptr) const;
		Array get_all_under_rect(const Rect2i &test_rect, const Variant &exception_metadata = nullptr) const;
		Array get_all_containing_point(const Vector2i &point, const Variant &exception_metadata = nullptr) const;
		void add(const Ref<QuadRect> &quad_rect);
		bool remove(const Variant &metadata);
		void _collapse();
	};

private:
	Ref<QuadNode> root;
	Vector2i size;

protected:
	static void _bind_methods();

public:
    QuadTree();
    ~QuadTree();
	void init(const Vector2i &size);
	Ref<QuadTree::QuadRect> get_first(const Variant &at, const Variant &exception_metadata = nullptr) const;
	Array get_all(const Variant &at, const Variant &exception_metadata = nullptr) const;
	void add(const Rect2i &rect, const Variant &metadata);
	bool remove(const Variant &metadata);
	bool is_empty() const;

	void set_root(const Ref<QuadNode> &new_root);
	Ref<QuadNode> get_root() const;
	void set_size(const Vector2i &new_root);
	Vector2i get_size() const;
};

#endif // QUAD_TREE_CLASS_H