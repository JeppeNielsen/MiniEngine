namespace Mini {
    class Clipper {
    public:
        Clipper();
        ~Clipper();
        
        void PushBegin();
        void PushEnd();
        void PopBegin();
        void PopEnd();
        
        bool UseDepth;
        
    private:
        int stackCount;
    };
}
