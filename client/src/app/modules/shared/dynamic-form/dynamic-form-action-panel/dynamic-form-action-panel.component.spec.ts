import { ComponentFixture, TestBed } from '@angular/core/testing';

import { DynamicFormActionPanelComponent } from './dynamic-form-action-panel.component';

describe('DynamicFormActionPanelComponent', () => {
  let component: DynamicFormActionPanelComponent;
  let fixture: ComponentFixture<DynamicFormActionPanelComponent>;

  beforeEach(async () => {
    await TestBed.configureTestingModule({
      imports: [DynamicFormActionPanelComponent]
    })
    .compileComponents();

    fixture = TestBed.createComponent(DynamicFormActionPanelComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
