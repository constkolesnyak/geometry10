#include <iostream>
#include <vector>
#include <cmath>
#include <sstream>
#include <array>
#include <iomanip>


class Point;

using namespace std;
using pVec = vector<Point>;
using ld = long double;
using li = long int;

template<class T>
int numerals_in_num(T&& arg) noexcept {
    static_assert(is_integral<T>::value, "0_o");
    int counter = 1;
    while (arg /= 10)
        ++counter;
    return counter;
}

template<class T>
int numerals_in_num(T const& arg) noexcept {
    return numerals_in_num(T(arg));
}

template<class T>
void print_prec(T&& arg) {
    if (!is_floating_point<T>::value) {
        cout << arg;
        return;
    }
    cout.precision(numerals_in_num(li(arg) + 5));
    cout << arg;
}

template<class T, class ...Types>
void print_prec(T&& arg1, Types&&... args) {
    print_prec(forward<T>(arg1));
    print_prec(forward<decltype(args)>(args)...);
}

inline string int_to_str(int arg) {
    stringstream ss;
    ss << arg;
    return ss.str();
}

inline void scan(ld& arg1) {
    string s;
    cin >> s;
    int nd = s.find('#');
    if (nd != string::npos) {
        string sn = s.substr(0, nd);
        arg1 = (sn.empty() ? 1 : stold(sn)) * sqrt(stold(string(s.substr(nd)).erase(0, 1)));
    }
    else {
        arg1 = stold(s);
    }
}

template<class ...Types>
void scan(ld& arg1, Types&... args) {
    scan(arg1);
    scan(args...);
}

inline ld nod(ld const& a, ld const& b) noexcept {
    return (b ? nod(b, fmodl(a, b)) : abs(a));
}

template<class ...Types>
ld nod(ld const& arg1, ld const& arg2, Types const&... args) noexcept {
    return nod(nod(arg1, arg2), args...);
}

class Point {
    ld _x{};
    ld _y{};
    ld _z{};
public:
    Point() noexcept {}

    Point(ld _x, ld _y, ld _z) noexcept
            : _x(_x), _y(_y), _z(_z) {}

    Point(string&& _name) {
        cout << "Введите координаты " << _name << ": ";
        scan(_x, _y, _z);
    }

    ld x() const noexcept {
        return _x;
    }

    ld y() const noexcept {
        return _y;
    }

    ld z() const noexcept {
        return _z;
    }

    void output() const {
        print_prec(_x, "  ", _y, "  ", _z, '\n');
    }
};

class Vector {
    ld _x{};
    ld _y{};
    ld _z{};
public:
    Vector(ld _x, ld _y, ld _z) noexcept
            : _x(_x), _y(_y), _z(_z) {}

    Vector(string&& _name) {
        cout << "Введите координаты " << _name << ": ";
        scan(_x, _y, _z);
    }

    Vector(Point const& p1, Point const& p2) noexcept {
        _x = p2.x() - p1.x();
        _y = p2.y() - p1.y();
        _z = p2.z() - p1.z();
    }

    ld x() const noexcept {
        return _x;
    }

    ld y() const noexcept {
        return _y;
    }

    ld z() const noexcept {
        return _z;
    }

    ld length() const noexcept {
        return sqrt(_x*_x + _y*_y + _z*_z);
    }

    void resize() noexcept {
        ld vNod = nod(_x, _y, _z);
        int counter = 0;
        this->for_each([&counter](ld a){ if (a < 0) ++counter; });
        if (counter >= 2)
            vNod *= -1;
        this->for_each([vNod](ld& a){ a /= vNod; });
    }

    template<class F>
    void for_each(F const& f) {
        f(_x); f(_y); f(_z);
    }

    void output() const {
        print_prec(_x, "  ", _y, "  ", _z, '\n');
    }

    Vector operator *(Vector const& v2) const noexcept {
        return Vector(
            _y*v2.z() - _z*v2.y(),
            _z*v2.x() - _x*v2.z(),
            _x*v2.y() - _y*v2.x()
        );
    }
};

class Plane {
    ld _A{};
    ld _B{};
    ld _C{};
    ld _D{};
public:
    Plane(ld _A, ld _B, ld _C, ld _D) noexcept
            : _A(_A), _B(_B), _C(_C), _D(_D) {}

    Plane(Vector const& normal, Point const& point) noexcept
            : _A(normal.x()), _B(normal.y()), _C(normal.z()) {
        _D = -_A*point.x() -_B*point.y() -_C*point.z();
    }

    Plane(Point p1, Point p2, Point p3) noexcept
            : Plane(Vector(p1, p2) * Vector(p1, p3), p2) {}

    Plane(string&& _name) {
        cout << "Введите коэффициэнты " << _name << ": ";
        scan(_A, _B, _C, _D);
    }

    ld A() const noexcept {
        return _A;
    }

    ld B() const noexcept {
        return _B;
    }

    ld C() const noexcept {
        return _C;
    }

    ld D() const noexcept {
        return _D;
    }

    bool includes(Point p) const noexcept {
        return abs(_A*p.x() + _B*p.y() + _C*p.z() + _D) < 0.001;
    }

    Vector normal() const noexcept {
        return Vector(_A, _B, _C);
    }

    void output() const {
        print_prec(_A, "  ", _B, "  ", _C, "  ", _D, '\n');
    }
};

inline ld cos_v1_v2(Vector const& v1, Vector const& v2) noexcept {
    return (
        ( v1.x()*v2.x() + v1.y()*v2.y() + v1.z()*v2.z() )/
        ( v1.length()*v2.length() )
    );
}

inline ld p_point_plane(Point const& point, Plane const& plane) noexcept {
    return (
        fabsl(
            ( plane.A()*point.x() + plane.B()*point.y() + plane.C()*point.z() + plane.D() )/
            plane.normal().length()
        )
    );
}

class S_points4bind {
    Point p1;
public:
    S_points4bind(Point const& p1) noexcept
            : p1(p1) {}
    ld operator ()(Point const& p2, Point const& p3, Point const& p4) const noexcept {
        return (Vector(p1, p3) * Vector(p2, p4)).length() / 2;
    }
};

inline ld S_points(Point const& p1, Point const& p2, Point const& p3) noexcept {
    return (Vector(p1, p2) * Vector(p1, p3)).length() / 2;
}

inline ld S_points(pVec const& pv) {
    size_t size = pv.size();
    if (size < 3)
        throw invalid_argument("Нужно минимум 3 points");
    ld S{};
    S_points4bind S_points4(pv[0]);
    int i = 1;

    if (size & 1) {
        while (i != size - 2)
            S += S_points4(pv.at(i), pv.at(i + 1), pv.at(i += 2));
        S += S_points(pv[0], pv.at(i), pv.at(i + 1));
    }
    else
        while (i != size - 1)
            S += S_points4(pv.at(i), pv.at(i + 1), pv.at(i += 2));
    return S;
}

int main() {
    enum class ShortComs {
        exit,
        V_cr_2point,
        V_resize,
        VxV,
        V_lenght,
        P_cr_normalIpoint,
        P_cr_3point,
        P_inc_points,
        cos_2V,
        p_pointIplane,
        p_aIb,
        S_points,
        size
    };
    array<string, (int)ShortComs::size> commands{
        "завершить работу программы",
        "создать вектор по 2 точкам",
        "сократить вектор",
        "умножить 2 вектора",
        "вычислить длину вектора",
        "создать плоскость по нормали и точке",
        "создать плоскость по 3 точкам",
        "вычислить, какие точки принадлежат плоскости",
        "вычислить cos(вектор1;вектор2)",
        "вычислить p(точка;плоскость)",
        "вычислить p(прямаяА;прямаяВ)",
        "вычислить S многоугольника"
    };

    for (;;) {
        int i{};
        int width = numerals_in_num(commands.size() - 1);
        for (string const& s : commands) {
            cout << setw(width) << left << i++ << " - " << s << endl;
        }

        int ncom;
        for (;;) {
            cout << "Операция: ";
            cin >> ncom;
            if (ncom >= 0 && ncom < commands.size())
                break;
            else
                cout << "Операции с таким номером нет\n"
                     << "Попробуйте ещё раз\n";
        }

        switch (ShortComs(ncom)) {
            case ShortComs::exit: {
                cout << "До свидания\n";
                exit(0);
            }
            case ShortComs::V_cr_2point: {
                Point p1("точки1"), p2("точки2");
                cout << "вектор = ";
                Vector(p1, p2).output();
                break;
            }
            case ShortComs::V_resize: {
                Vector vec("вектора");
                vec.resize();
                cout << "Сокращённый вектор = ";
                vec.output();
                break;
            }
            case ShortComs::VxV: {
                Vector vec1("вектора1"), vec2("вектора2");
                cout << "вектор1 x вектор2 = ";
                (vec1 * vec2).output();
                break;
            }
            case ShortComs::V_lenght: {
                Vector vec("вектора");
                print_prec("Длина вектора = ", vec.length(), '\n');
                break;
            }
            case ShortComs::P_cr_normalIpoint: {
                Vector normal("нормали");
                Point point("точки");
                cout << "Коэффициэнты плоскости: ";
                Plane(normal, point).output();
                break;
            }
            case ShortComs::P_cr_3point: {
                Point p1("точки1"), p2("точки2"), p3("точки3");
                cout << "Коэффициэнты плоскости: ";
                Plane(p1, p2, p3).output();
                break;
            }
            case ShortComs::P_inc_points: {
                Plane plane("плоскости");
                int num;
                for (;;) {
                    cout << "Введите кол-во точек: ";
                    cin >> num;
                    if (num >= 1)
                        break;
                    else
                        cout << "Нужна минимум 1 point\n"
                             << "Попробуйте ещё раз\n";
                }
                int np{};
                bool res{1};
                while (num--) {
                    if (!plane.includes(Point("точки" + int_to_str(++np)))) {
                        cout << "Эта point не принадлежит плоскости\n";
                        res = 0;
                        break;
                    }
                }
                if (res)
                    cout << "Все точки принадлежат плоскости\n";
                break;
            }
            case ShortComs::cos_2V: {
                Vector vec1("вектора1"), vec2("вектора2");
                print_prec("cos(вектор1;вектор2) = ", cos_v1_v2(vec1, vec2), '\n');
                break;
            }
            case ShortComs::p_pointIplane: {
                Point point("точки");
                Plane plane("плоскости");
                print_prec("p(точка;плоскость) = ", p_point_plane(point, plane), '\n');
                break;
            }
            case ShortComs::p_aIb: {
                Vector pa("напр_вектораА"), pb("напр_вектораВ");
                Point pointA("точки_наА"), pointB("точки_наВ");
                print_prec("p(А;В) = ", p_point_plane(pointA, Plane(pa * pb, pointB)), '\n');
                break;
            }
            case ShortComs::S_points: {
                int num;
                for (;;) {
                    cout << "Введите кол-во точек: ";
                    cin >> num;
                    if (num >= 3)
                        break;
                    else
                        cout << "Нужно минимум 3 points\n"
                             << "Попробуйте ещё раз\n";
                }
                pVec vp;
                int np{};
                while (num--) {
                    vp.push_back("точки" + int_to_str(++np));
                }
                print_prec("S = ", S_points(vp), '\n');
                break;
            }
        }
        cout << "-------------------" << endl << endl;
    }
}
