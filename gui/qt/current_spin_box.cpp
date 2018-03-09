#include "current_spin_box.h"

#include <QLineEdit>
#include <QSpinBox>
#include <QtMath>
#include <QRegExpValidator>
#include <QDebug>
#include <iostream> //tmphax

current_spin_box::current_spin_box(QWidget * parent)
  : QDoubleSpinBox(parent)
{
  connect(this, QOverload<double>::of(&valueChanged),
    this, &set_code_from_value);
  connect(this, &editingFinished, this, &editing_finished);
}

void current_spin_box::set_mapping(const QMap<int, int> & mapping)
{
  this->mapping = mapping;
  fix_code_if_not_allowed();
  set_value_from_code();
}

// Ensures that 'code' is one of the allowed codes, or -1 if there are no
// allowed codes.  Note that this is not as smart as it could be because it
// doesn't take into account the value corresponding to a code that is now
// disallowed, but in our applications we anticipate the caller settings the
// value explcitly to something if that ever happens.
void current_spin_box::fix_code_if_not_allowed()
{
  if (mapping.contains(code))
  {
    return;
  }

  if (mapping.size() == 0)
  {
    code = -1;
    return;
  }

  // Find the code corresponding to the minimum current and use that.
  int best_code = -1;
  int best_current = std::numeric_limits<int>::max();
  for(int candidate_code : mapping.keys())
  {
    int current = mapping.value(candidate_code);
    if (current < best_current)
    {
      best_code = candidate_code;
      best_current = current;
    }
  }
  code = best_code;
}

// Finds the code for the highest current less than the current currently
// displayed in the box, and set 'code' to that.  Sets 'code' to -1 if the
// mapping is empty.
void current_spin_box::set_code_from_value()
{
  int current_from_value = qRound(value());

  int best_code = -1;
  int best_current = -1;
  for(int candidate_code : mapping.keys())
  {
    int current = mapping.value(candidate_code);
    if (best_code == -1 || (current <= current_from_value && current > best_current))
    {
      best_code = candidate_code;
      best_current = current;
    }
  }
  code = best_code;
}

void current_spin_box::set_value_from_code()
{
  setValue(mapping.value(code, 0));
}

void current_spin_box::editing_finished()
{
  set_code_from_value();
  set_value_from_code();
}

void current_spin_box::stepBy(int step_value)
{
  int current = qRound(value());

  while (step_value > 0)
  {
    // Select the smallest code/current where the current is greater.
    int best_code = -1;
    int best_current = std::numeric_limits<int>::max();
    for(int candidate_code : mapping.keys())
    {
      int candidate_current = mapping.value(candidate_code);
      if (candidate_current > current && candidate_current < best_current)
      {
        best_code = candidate_code;
        best_current = candidate_current;
      }
    }
    if (best_code == -1)
    {
      // We can't step up any more, we reached the top.
      break;
    }
    code = best_code;
    step_value--;
  }

  while (step_value < 0)
  {
    // Select the greatest code/current where the current is smaller.
    int best_code = -1;
    int best_current = -1;
    for(int candidate_code : mapping.keys())
    {
      int candidate_current = mapping.value(candidate_code);
      if (candidate_current < current && candidate_current > best_current)
      {
        best_code = candidate_code;
        best_current = candidate_current;
      }
    }
    if (best_code == -1)
    {
      // We can't step down down any more, we reached the bottom.
      break;
    }
    code = best_code;
    step_value++;
  }

  set_value_from_code();

  selectAll();
}

double current_spin_box::valueFromText(const QString & text) const
{
  QString copy = text.toUpper();

  // TODO: if they entered it in A, convert to mA

  return copy.toDouble();
}

QValidator::State current_spin_box::validate(QString & input, int & pos) const
{
  // TODO: allow it to end with just "M" or "A"
  QRegExp r = QRegExp("(\\d{0,6})(\\.\\d{0,4})?(\\s*)(m|ma|Ma|mA|MA)?");

  if (r.exactMatch(input))
  {
    return QValidator::Acceptable;
  }
  else
  {
    return QValidator::Invalid;
  }
}
