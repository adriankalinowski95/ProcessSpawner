import { ComponentFixture, TestBed } from '@angular/core/testing';

import { DialogActionPanelComponent } from './dialog-action-panel.component';

describe('DialogActionPanelComponent', () => {
  let component: DialogActionPanelComponent;
  let fixture: ComponentFixture<DialogActionPanelComponent>;

  beforeEach(async () => {
    await TestBed.configureTestingModule({
      imports: [DialogActionPanelComponent]
    })
    .compileComponents();

    fixture = TestBed.createComponent(DialogActionPanelComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
