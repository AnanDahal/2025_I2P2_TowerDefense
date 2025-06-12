//
// Created by user on 5/25/2025.
//

#ifndef BIGGERCARRIERENEMY_H
#define BIGGERCARRIERENEMY_H
class BiggerCarrierEnemy : public Enemy {
public:
    BiggerCarrierEnemy(int x, int y);
protected:
    void OnExplode() override;
};
#endif //BIGGERCARRIERENEMY_H
