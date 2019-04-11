#pragma once
// --------------------------------------------------------------------------------
// Name        : RectPlacement.h
// Description : A class that allocates subrectangles into power-of-2 rectangles
//               (C) Copyright 2000-2002 by Javier Arevalo
//               This code is free to use and modify for all purposes
// --------------------------------------------------------------------------------

#include <vector>

namespace Mini {
    class RectPacker {
    public:

    // Helper classes
    struct TPos {
      int x, y;

      TPos() { }
      TPos(int _x, int _y): x(_x), y(_y) { }

      bool operator ==(const TPos &p) const { return x == p.x && y == p.y; }
    };

    struct TRect: public TPos {
      int w, h;

      TRect() { }
      TRect(int _x, int _y, int _w, int _h): TPos(_x, _y), w(_w > 0? _w : 0), h(_h > 0? _h : 0) { }

      bool Contains   (const TPos &p)  const { return (p.x >= x && p.y >= y &&
                                                       p.x < (x+w) && p.y < (y+h)); }
      bool Contains   (const TRect &r) const { return (r.x >= x && r.y >= y &&
                                                       (r.x+r.w) <= (x+w) && (r.y+r.h) <= (y+h)); }
      bool Intersects (const TRect &r) const { return w > 0 && h > 0 && r.w > 0 && r.h > 0 &&
                                                      ((r.x+r.w) > x && r.x < (x+w) &&
                                                       (r.y+r.h) > y && r.y < (y+h)); }

      static bool Greater(const TRect &a, const TRect &b) { return (a.w > b.w && a.w > b.h) ||
                                                                   (a.h > b.w && a.h > b.h); }
    };

    using CPosArray = std::vector<TPos>;
    using CRectArray = std::vector<TRect>;

    // ---------------------

    RectPacker()                    { Init(); }
    ~RectPacker()                   { End(); }

    void      Init    (int w = 1, int h = 1);
    void      End     ();
    bool      IsOk    ()                      const { return m_size.w > 0; }

    int       GetW    ()                      const { return m_size.w; }
    int       GetH    ()                      const { return m_size.h; }
    long      GetArea ()                      const { return m_area; }
    long      GetTotalArea ()                 const { return m_size.w*m_size.h; }

    bool AddAtEmptySpotAutoGrow (TRect *pRect, int maxW, int maxH);

    private:
        TRect       m_size;
        CRectArray  m_vRects;
        CPosArray   m_vPositions;
        long        m_area;

        bool IsFree                 (const TRect &r) const;
        void AddPosition            (const TPos &p);
        void AddRect                (const TRect &r);
        bool AddAtEmptySpot         (TRect &r);
    };
}
