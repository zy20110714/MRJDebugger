#include "showitem.h"

ShowItem::ShowItem(bool isCheck, unsigned int item, QPen penForThisItem, int queueSize)
{
    IsCheck = isCheck;
    Item = item;
    PenForThisItem = penForThisItem;
    sq = new ShowQueue(queueSize);
}
