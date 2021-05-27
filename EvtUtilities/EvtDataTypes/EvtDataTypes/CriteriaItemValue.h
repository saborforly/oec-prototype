#ifndef CRITERIA_ITEM_VALUE_H
#define CRITERIA_ITEM_VALUE_H

class CriteriaItemValue {
  public:
    // Standard constructor and destructor
    CriteriaItemValue();
    virtual ~CriteriaItemValue();

    // Reset CriteriaItemValue status
    void reset() { m_stat = false; }
    // Set the value available
    void setValue(float value) {
      m_value = value;
      m_stat  = true;
    }
    // Retrieve the value
    bool getValue(float& value) const {
      if (m_stat) value = m_value;
      return m_stat;
    }

  private:
    float m_value;
    bool  m_stat;
};
#endif
