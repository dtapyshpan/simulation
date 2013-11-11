#include "../include/ContextMenu.h" 

ContextMenu::ContextMenu()
{
  m_pmnu = new QMenu();
  m_pmnu->addAction( "Edit" );

  connect( m_pmnu, SIGNAL( triggered( QAction * ) ), SLOT( slotActivated( QAction * ) ) );
}

void ContextMenu::slotActivated( QAction *pAction )
{
  printf("slotActivated\n");
}

void ContextMenu::showContextMenu( QContextMenuEvent *pe )
{
  m_pmnu->exec( pe->globalPos() );
}
