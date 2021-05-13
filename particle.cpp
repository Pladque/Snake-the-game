
class Particle
{
public:
    float speedX;
    float speedY;
    float ttl;
    float size;
    float color;
    float velocityX;
    float velocityY;

    Particle* next;

    int positionX = 0, positionY = 0;

    
    Particle(float newSpeedX, float newSpeedY, float newTTL, float newSize
    ,int newColor, float newVelocityX, float newVelocityY)
    {
        this->speedX = newSpeedX;
        this->speedY = newSpeedY;
        this->ttl = newTTL;
        this->size = newSize;
        this->color = newColor;
        this->velocityX = newVelocityX;
        this->velocityY = newVelocityY;
        this->next = nullptr;
    }

};


class PartycleSystem
{
    Particle* particles;
    int positionX;
    int positionY;
    float delay;

    bool ready_to_blow_up;
    bool already_died;

public:
    PartycleSystem(){
        this->particles = nullptr;
        this-> positionX = 0;
        this->positionY = 0;
        this->delay = 0.0;
        this->ready_to_blow_up = false;
        this->already_died = true;
    }
    PartycleSystem(Particle *newShape, int newPosX, 
                    int newPosY, float newDelay)
    {
        this->particles = newShape;
        this-> positionX = newPosX;
        this->positionY = newPosY;
        this->delay = newDelay;

        this->ready_to_blow_up = true;
        this->already_died = false;
    }

    void blowUp()
    {
        if(this->delay <= 0)
        {
            this->ready_to_blow_up = false;
            Particle* curr = particles;

            while(curr != nullptr)
            {
                curr->positionX += curr->speedX;
                curr->positionY += curr->speedY;

                curr->speedX += curr->velocityX;
                curr->speedY += curr->velocityY;
                curr->velocityY += 0.0000;           //making falling overtime

                curr->ttl -= 0.1;
                curr->size *= ((100000 - curr->ttl) / 100000) > 0 ? 
                ((100000 - curr->ttl) / 100000) : -((100000 - curr->ttl) / 100000);  //making smaler over time

                curr = curr->next;

            }

        }
    }

    Particle* getFirstParticle()
    {
        return this->particles;
    }

    void setPosition(float newPosX, float newPosY)
    {
        this->positionX = newPosX;
        this->positionY = newPosY;
    }

    float getPosX()
    {
        return this->positionX;
    }

    float getPosY()
    {
        return this->positionY;
    }



};
