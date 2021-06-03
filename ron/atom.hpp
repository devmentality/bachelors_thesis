//
// Created by gritzko on 3/7/21.
//

#ifndef PULSAR_ATOM_HPP
#define PULSAR_ATOM_HPP
#include "op.hpp"

namespace ron {

class Atom : public Double {
   public:
    static const Atom NIL;
    static constexpr unsigned TYPE_BIT_SHIFT = 62U;
    static constexpr Word ID_FLAGS{Word(ID) << TYPE_BIT_SHIFT};
    static constexpr Word INT_FLAGS{Word(INT) << TYPE_BIT_SHIFT};
    static constexpr Word STRING_FLAGS{Word(STRING) << TYPE_BIT_SHIFT};
    static constexpr Word FLOAT_FLAGS{Word(FLOAT) << TYPE_BIT_SHIFT};

   protected:
    void Release() noexcept {
        if (Type() != STRING || !first) return;
        free((void*)AsCodepoints());
        first = 0;
        second = 0;
    }

    Index StringSize() const { return Index(second); }

    const Codepoint* AsCodepoints() const {
        return reinterpret_cast<Codepoint*>(first);
    }

    void Dup() {
        auto p = StringAlloc(StringSize());
        StringCopy(p, AsCodepoints(), StringSize());
        first = reinterpret_cast<Word>(p);
    }

   public:
    Atom() = default;

    Atom(const Atom& b) noexcept : Double{b.first, b.second} {
        if (Type() == STRING && first) Dup();
    }

    Atom(Atom&& b) noexcept : Double{b.first, b.second} {
        b.first = 0;
        b.second = 0;
    }

    explicit Atom(Uuid id) : Atom{} { *this = id; }

    ~Atom() { Release(); }

    bool IsNil() const { return first == 0 && second == 0; }

    Atom& operator=(const Atom& b) {
        first = b.first;
        second = b.second;
        if (Type() == STRING && first) Dup();
        return *this;
    }

    Atom& operator=(Atom&& b) {
        first = b.first;
        second = b.second;
        b.first = b.second = 0;
        return *this;
    }

    bool operator==(const Atom& b) const {
        if (second != b.second) return false;
        if (Type() != STRING) return first == b.first;
        return 0 ==
               StringCompare(AsCodepoints(), b.AsCodepoints(), StringSize());
    }

    bool operator<(const Atom& b) const {
        if (Type() != b.Type()) return Type() < b.Type();
        switch (Type()) {
            case ID:
                return AsUuid() < b.AsUuid();
            case INT:
                return AsInteger() < b.AsInteger();
            case STRING:
                return -1 == StringCompare(AsCodepoints(), b.AsCodepoints(),
                                           StringSize());
            case FLOAT:
                return AsFloat() < b.AsFloat();
            default:
                abort();
        }
    }

    ATOM Type() const { return ATOM(second >> TYPE_BIT_SHIFT); }

    Float AsFloat() const {
        assert(Type() == FLOAT);
        return reinterpret_cast<const Float&>(first);
    }

    Integer AsInteger() const {
        assert(Type() == INT);
        return reinterpret_cast<const Integer&>(first);
    }

    Uuid AsUuid() const {
        assert(Type() == ID);
        return Uuid{first, second & ~(3UL << TYPE_BIT_SHIFT)};
    }

    span32c AsString() const {
        assert(Type() == STRING);
        auto p = AsCodepoints();
        return span32c{(uint32_t*)p, second & WORD_PAYLOAD_BITS};
    }

    std::string AsUtf8String() const {
        std::string ret{};
        auto s32 = AsString();
        SaveUtf8(ret, s32);
        return ret;
    }

    bool operator==(Integer value) const {
        return Type() == INT && AsInteger() == value;
    }

    bool operator==(Float value) const {
        return Type() == FLOAT && AsFloat() == value;
    }

    bool operator==(span32c str) const {
        return Type() == STRING && AsString() == str;
    }

    //    std::string AsRONt() const {
    //        TextStringWriter writer{};
    //        switch (Type()) {
    //            case ID:
    //                AsUuid().Save(writer);
    //                break;
    //            case INT:
    //                writer.SaveInteger(AsInteger());
    //                break;
    //            case STRING:
    //                writer.SaveEscapedString(AsString());
    //                break;
    //            case FLOAT:
    //                writer.SaveFloat(AsFloat());
    //                break;
    //        }
    //        return writer.Release();
    //    }

    Atom& operator=(Float aFloat) {
        Release();
        first = reinterpret_cast<Word&>(aFloat);
        second = FLOAT_FLAGS;
        return *this;
    }

    Atom& operator=(Integer i) {
        Release();
        first = reinterpret_cast<Word&>(i);
        second = INT_FLAGS;
        return *this;
    }

    Atom& operator=(Uuid id) {
        Release();
        first = id.first;
        second = id.second | ID_FLAGS;
        return *this;
    }

    Atom& operator=(span32c str) {
        Release();
        auto p = StringAlloc(str.size());
        StringCopy(p, (Codepoint*)str.data(), str.size());
        first = reinterpret_cast<Word>(p);
        second = Word(str.size()) | STRING_FLAGS;
        return *this;
    }

    Atom& operator=(const std::string& ront) {
        auto t = Tuple::As(ront);
        if (!t.IsSingleAtom()) {
            string32 str{};
            ParseUtf8(str, ront);
            *this = as_span32c(str);
            return *this;
        }
        Tuple::Box atom{};
        t.ReadAtoms().ReadAtom(atom);
        if (atom.Type() == ID) {
            *this = atom.UuidAt(0);
        } else if (atom.Type() == INT) {
            *this = atom.IntegerAt(0);
        } else if (atom.Type() == FLOAT) {
            *this = atom.FloatAt(0);
        } else {
            *this = atom.String();
        }
        return *this;
    }

    PROC Set(Tuple::Box atom) {
        switch (atom.Type()) {
            case ID:
                NEED(atom.Size() == 1, OUTORANGE.Ext(BASE_S));
                (*this) = atom.UuidAt(0);
                DONE;
            case INT:
                NEED(atom.Size() == 1, OUTORANGE.Ext(BASE_S));
                (*this) = atom.IntegerAt(0);
                DONE;
            case FLOAT:
                NEED(atom.Size() == 1, OUTORANGE.Ext(BASE_S));
                (*this) = atom.FloatAt(0);
                DONE;
            case STRING:
                (*this) = atom.String();
            default:
                abort();
        }
        DONE;
    }

    Tuple::Box AsBox() const {
        switch (Type()) {
            case INT:
            case FLOAT:
                return Tuple::Box{Type(), 1, (Word*)this};
            case STRING:
                return Tuple::Box{STRING, Index(second), (Word*)first};
            case ID:
                return Tuple::Box{ID, 1, (Word*)this};
            default:
                abort();
        }
    }

    /** a coercing accessor */
    Uuid ToUuid() const;
    string32 ToString() const;
    Float ToFloat() const;
    Integer ToInteger() const;
};

}  // namespace ron

#endif  // PULSAR_ATOM_HPP
